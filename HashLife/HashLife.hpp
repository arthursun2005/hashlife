//
//  HashLife.hpp
//  HashLife
//
//  Created by Arthur Sun on 4/16/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef HashLife_hpp
#define HashLife_hpp

#include <cstdlib>
#include <unordered_map>
#include <array>
#include <vector>
#include "LifeRule.h"
#include "HashMap.h"

#define cell_types 2

static int _hash_base_scales[4] = {
    1,
    cell_types,
    cell_types * cell_types,
    cell_types * cell_types * cell_types
};

class HashLife
{
    
protected:
    
    std::allocator<Node> allocator;
    
    Node* nodes;
    
    LifeRule rule;
    
    int root;

    int alloc();
    int allocChunk(int);
    int alloc(int);
    void alloc_children(int);
    void reserve(int);
    
    void copy(int, int);
    int clone(int);
    int cloneAligned(int);
    
    void free(int);
    void trash(int);
    
    int apply(int);
    
    void computeHash(int);
    void computeAllHashs(int);
    
    void expandOnce();
    
    
    
    int ref_h(int a, int b) {
        int n = alloc();
        
        nodes[n][0] = nodes[a][1];
        nodes[n][1] = nodes[b][0];
        nodes[n][2] = nodes[a][3];
        nodes[n][3] = nodes[b][2];
        
        nodes[n].level = nodes[a].level;
        
        return n;
    }
    
    int ref_v(int a, int b) {
        int n = alloc();
        
        nodes[n][0] = nodes[a][2];
        nodes[n][1] = nodes[a][3];
        nodes[n][2] = nodes[b][0];
        nodes[n][3] = nodes[b][1];
        
        nodes[n].level = nodes[a].level;
        
        return n;
    }
    
    int ref_h2(int a, int b) {
        int n = alloc();
        
        nodes[n][0] = nodes[nodes[a][1]][3];
        nodes[n][1] = nodes[nodes[b][0]][2];
        nodes[n][2] = nodes[nodes[a][3]][1];
        nodes[n][3] = nodes[nodes[b][2]][0];
        
        nodes[n].level = nodes[a].level - 1;
        
        return n;
    }
    
    int ref_v2(int a, int b) {
        int n = alloc();
        
        nodes[n][0] = nodes[nodes[a][2]][3];
        nodes[n][1] = nodes[nodes[a][3]][2];
        nodes[n][2] = nodes[nodes[b][0]][1];
        nodes[n][3] = nodes[nodes[b][1]][0];
        
        nodes[n].level = nodes[a].level - 1;
        
        return n;
    }
    
    int ref_cnt(int a, int b, int c, int d) {
        int n = alloc();
        
        nodes[n][0] = nodes[a][3];
        nodes[n][1] = nodes[b][2];
        nodes[n][2] = nodes[c][1];
        nodes[n][3] = nodes[d][0];
        
        nodes[n].level = nodes[a].level;
        
        return n;
    }
    
    int ref_cnt2(int a, int b, int c, int d) {
        int n = alloc();
        
        nodes[n][0] = nodes[nodes[a][3]][3];
        nodes[n][1] = nodes[nodes[b][2]][2];
        nodes[n][2] = nodes[nodes[c][1]][1];
        nodes[n][3] = nodes[nodes[d][0]][0];
        
        nodes[n].level = nodes[a].level - 1;
        
        return n;
    }
    
    int ref_node(int a, int b, int c, int d) {
        int n = alloc();
        
        nodes[n][0] = a;
        nodes[n][1] = b;
        nodes[n][2] = c;
        nodes[n][3] = d;
        
        nodes[n].level = nodes[a].level + 1;
        
        return n;
    }
    
    int ref_cnt(int n) {
        if(nodes[n].isLeaf())
            return -1;
        
        return ref_cnt(nodes[n][0], nodes[n][1], nodes[n][2], nodes[n][3]);
    }
    
    int ref_cnt2(int n) {
        if(nodes[n].level < 3)
            return -1;
        
        return ref_cnt2(nodes[n][0], nodes[n][1], nodes[n][2], nodes[n][3]);
    }
    
    void save_results(int key, int value) {
        cache.set(key, value);
    }
    
    using Map1 = std::unordered_map<int, int, std::hash<Node>, std::equal_to<Node>>;
    
    using Map2 = HashMap<>;
    
    using Map = Map2;
    
    Map cache;
    
    int solve(int);
    int solveOnce(int);
    
    int next;
    
    int count;
    int capacity;
    
public:
    
    HashLife() : count(0), capacity(0x10000), next(0) {
        nodes = allocator.allocate(capacity);
        
        cache.setNodes(nodes);
        
        for(int i = 0; i < capacity - 1; ++i)  {
            nodes[i].next = i + 1;
        }
        
        nodes[capacity - 1].next = -1;
        
        root = alloc(6);
    }
    
    HashLife(const HashLife& life) {
    }
    
    ~HashLife() {
        allocator.deallocate(nodes, capacity);
    }
    
    inline int size() const
    {
        return count;
    }
    
    const Map& getCache() const
    {
        return cache;
    }
    
    void step();
};

#endif /* HashLife_hpp */
