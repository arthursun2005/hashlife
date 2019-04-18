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
        return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
    }
    
    bool isLeaf() const
    {
        assert(level > 0);
        return level == 1;
    }
};

#endif /* Node_h */
