//
//  Key.h
//  HashLife
//
//  Created by Arthur Sun on 4/17/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef Key_h
#define Key_h

#include <string>

template <size_t N>
struct _Key
{
    std::array<char, N> data;
    
    _Key() {}
    
    friend inline bool operator == (const _Key<N>& a, const _Key<N>& b)
    {
        for(int i = 0; i < N; ++i) {
            if(a.data[i] != b.data[i])
                return false;
        }
        return true;
    }
    
    std::string str() const
    {
        return std::string(data.begin(), data.end());
    }
};

template <size_t N>
struct std::hash <_Key<N>>
{
    inline size_t operator () (const _Key<N>& key) const
    {
        size_t hash = key.data[0];
        
        for(size_t i = 1; i < N; ++i)
            hash ^= key.data[i] << ((i * CHAR_BIT) % __SIZE_WIDTH__);
        
        return hash;
    }
};

#endif /* Key_h */
