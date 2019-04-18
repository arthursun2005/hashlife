//
//  HashMap.h
//  HashLife
//
//  Created by Arthur Sun on 4/18/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef HashMap_h
#define HashMap_h

#include "Node.h"
#include <vector>

template <>
struct std::hash <Node>
{
    Node* nodes;
    
    inline size_t operator () (int key) const
    {
        return nodes[key].hash;
    }
};

template <>
struct std::equal_to <Node>
{
    Node* nodes;
    
    inline bool operator () (const int& a, const int& b) {
        if(nodes[a].level != nodes[b].level)
            return false;
        
        if(nodes[a].isLeaf()) {
            return nodes[a] == nodes[b];
        }
        
        return operator()(nodes[a][0], nodes[b][0]) && operator()(nodes[a][1], nodes[b][1]) && operator()(nodes[a][2], nodes[b][2]) && operator()(nodes[a][3], nodes[b][3]);
    }
};

template <size_t N = 0x1000000>
class HashMap
{
    
protected:
    
    std::hash<Node> hasher;
    std::equal_to<Node> judger;
    
    using Pair = std::pair<int, int>;
    
    using List = std::vector<Pair>;
    
    std::allocator<List> allocator;
    
    List* list;
    
    Node* nodes;
    
public:
    
    int count = 0;
    
    HashMap() {
        list = allocator.allocate(N);
        
        for(int i = 0; i < N; ++i) {
            list[i].clear();
        }
    }
    
    ~HashMap() {
        allocator.deallocate(list, N);
    }
    
    void setNodes(Node* _nodes) {
        nodes = _nodes;
        
        hasher.nodes = nodes;
        judger.nodes = nodes;
    }
    
    bool erase(int key) {
        size_t position = hasher(key)%N;
        
        List& k = list[position];
        
        for (auto it = k.begin(); it != k.end(); ++it) {
            if(judger(key, it->first)) {
                --count;
                k.erase(it);
                return true;
            }
        }
        
        return false;
    }
    
    bool set(int key, int value) {
        size_t position = hasher(key)%N;
        
        List& k = list[position];
        
        for(Pair& x : k) {
            if(judger(key, x.first)) {
                return true;
            }
        }

        k.push_back(Pair(key, value));
        
        ++count;
        
        return false;
    }
    
    bool find(int key, int* value) {
        size_t position = hasher(key)%N;
        List& k = list[position];
        
        for(Pair& x : k) {
            if(judger(key, x.first)) {
                *value = x.second;
                return true;
            }
        }
        
        return false;
    }
    
};

#endif /* HashMap_h */
