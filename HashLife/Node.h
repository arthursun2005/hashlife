//
//  Node.h
//  HashLife
//
//  Created by Arthur Sun on 4/18/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef Node_h
#define Node_h

struct Node
{
    size_t hash;
    
    int level;
    
    int next;
    
    std::array<int, 4> children;
    
    Node() {}
    
    int& operator [] (uint8_t i)
    {
        return children[i];
    }
    
    const int& operator [] (uint8_t i) const
    {
        return children[i];
    }
    
    friend bool operator == (const Node& a, const Node& b)
    {
        return a.children == b.children;
    }
    
    inline int size() const
    {
        return ((1 << (level * 2)) - 1) / 3;
    }
    
    bool isLeaf() const
    {
        assert(level > 0);
        return level == 1;
    }
};

#endif /* Node_h */
