//
//  HashLife.cpp
//  HashLife
//
//  Created by Arthur Sun on 4/16/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "HashLife.hpp"

void HashLife::step() {
    if(root == -1) return;
    
    computeAllHashs(root);
    
    int result = solve(root);
    
    if(nodes[root].level > 2) {
        trash(nodes[nodes[root][0]][3]);
        trash(nodes[nodes[root][1]][2]);
        trash(nodes[nodes[root][2]][1]);
        trash(nodes[nodes[root][3]][0]);
    }
    
    nodes[nodes[root][0]][3] = nodes[result][0];
    nodes[nodes[root][1]][2] = nodes[result][1];
    nodes[nodes[root][2]][1] = nodes[result][2];
    nodes[nodes[root][3]][0] = nodes[result][3];
    
    free(result);
}

int HashLife::solve(int node) {
    assert(!nodes[node].isLeaf());
    
    int k;
    bool found = cache.find(node, &k);
    if(found) {
        int n = alloc(nodes[node].level - 1);
        copy(n, k);
        return n;
    }
    
    if(nodes[node].level == 2) {
        int n = apply(node);
        
        save_results(clone(node), clone(n));
        
        return n;
    }else{
        /*
        int w0 = ref_cnt2(node);
        int w1 = ref_h2(nodes[node][0], nodes[node][1]);
        int w2 = ref_h2(nodes[node][2], nodes[node][3]);
        int w3 = ref_v2(nodes[node][0], nodes[node][2]);
        int w4 = ref_v2(nodes[node][1], nodes[node][3]);
        */
        
        int s0 = ref_cnt(node);
        int s1 = ref_h(nodes[node][0], nodes[node][1]);
        int s2 = ref_h(nodes[node][2], nodes[node][3]);
        int s3 = ref_v(nodes[node][0], nodes[node][2]);
        int s4 = ref_v(nodes[node][1], nodes[node][3]);
        
        /*
        int c0 = ref_cnt(nodes[node][0]);
        int c1 = ref_cnt(nodes[node][1]);
        int c2 = ref_cnt(nodes[node][2]);
        int c3 = ref_cnt(nodes[node][3]);
        
        int w5 = ref_node(c0, w1, w3, w0);
        int w6 = ref_node(w1, c1, w0, w4);
        int w7 = ref_node(w3, w0, c2, w2);
        int w8 = ref_node(w0, w4, w2, c3);
        */
        
        int n0 = solve(nodes[node][0]);
        int n1 = solve(s1);
        int n2 = solve(nodes[node][1]);
        int n3 = solve(s3);
        int n4 = solve(s0);
        int n5 = solve(s4);
        int n6 = solve(nodes[node][2]);
        int n7 = solve(s2);
        int n8 = solve(nodes[node][3]);
        
        int c1 = ref_node(n0, n1, n3, n4);
        int c2 = ref_node(n1, n2, n4, n5);
        int c3 = ref_node(n3, n4, n6, n7);
        int c4 = ref_node(n4, n5, n7, n8);
        
        int f0 = solve(c1);
        int f1 = solve(c2);
        int f2 = solve(c3);
        int f3 = solve(c4);
        
        int f = ref_node(f0, f1, f2, f3);
        
        save_results(clone(node), clone(f));

        free(s0);
        free(s1);
        free(s2);
        free(s3);
        free(s4);
        
        free(c1);
        free(c2);
        free(c3);
        free(c4);
        
        trash(n0);
        trash(n1);
        trash(n2);
        trash(n3);
        trash(n4);
        trash(n5);
        trash(n6);
        trash(n7);
        trash(n8);
        
        
        return f;
        
        /*
        solve(c5, w5);
        solve(c6, w6);
        solve(c7, w7);
        solve(c8, w8);
        
        save_results(clone(write), read);
         
        
        free(r0);
        free(r1);
        free(r2);
        free(r3);
        free(r4);
        
        free(w0);
        free(w1);
        free(w2);
        free(w3);
        free(w4);
        
        free(c0);
        free(c1);
        free(c2);
        free(c3);
        
        free(w5);
        free(w6);
        free(w7);
        free(w8);
        
        trash(_c0);
        trash(_c1);
        trash(_c2);
        trash(_c3);
        
        trash(_w0);
        trash(_w1);
        trash(_w2);
        trash(_w3);
        trash(_w4);
        
        free(c5);
        free(c6);
        free(c7);
        free(c8);
         */
    }
}

void HashLife::computeAllHashs(int node) {
    size_t hash = nodes[node].level;
    if(nodes[node].isLeaf()) {
        hash += nodes[node][0] * _hash_base_scales[0];
        hash += nodes[node][1] * _hash_base_scales[1];
        hash += nodes[node][2] * _hash_base_scales[2];
        hash += nodes[node][3] * _hash_base_scales[3];
    }else{
        computeAllHashs(nodes[node][0]);
        computeAllHashs(nodes[node][1]);
        computeAllHashs(nodes[node][2]);
        computeAllHashs(nodes[node][3]);
        
        hash += nodes[nodes[node][0]].hash * _hash_base_scales[0];
        hash += nodes[nodes[node][1]].hash * _hash_base_scales[1];
        hash += nodes[nodes[node][2]].hash * _hash_base_scales[2];
        hash += nodes[nodes[node][3]].hash * _hash_base_scales[3];
    }
    nodes[node].hash = hash;
}

int HashLife::apply(int n) {
    assert(nodes[n].level == 2);
    int k = alloc();
    
    const Node& node = nodes[n];
    
    const Node& nw = nodes[node[0]];
    const Node& ne = nodes[node[1]];
    const Node& sw = nodes[node[2]];
    const Node& se = nodes[node[3]];
    
    int s0 = nw[0] + nw[1] + ne[0] + nw[2] + ne[2] + sw[0] + sw[1] + se[0];
    int s1 = nw[1] + ne[0] + ne[1] + nw[3] + ne[3] + sw[1] + se[0] + se[1];
    int s2 = nw[2] + nw[3] + ne[2] + sw[0] + se[0] + sw[2] + sw[3] + se[2];
    int s3 = nw[3] + ne[2] + ne[3] + sw[1] + se[1] + sw[3] + se[2] + se[3];
    
    nodes[k][0] = rule[nw[3]][s0];
    nodes[k][1] = rule[ne[2]][s1];
    nodes[k][2] = rule[sw[1]][s2];
    nodes[k][3] = rule[se[0]][s3];
    
    nodes[k].level = 1;
    
    return k;
}

int HashLife::alloc() {
    if(next == -1) {
        assert(count == capacity);
        
        Node* oldNodes = nodes;
        
        capacity *= 2;
        nodes = allocator.allocate(capacity);
        
        cache.setNodes(nodes);
        
        memcpy(nodes, oldNodes, sizeof(Node) * count);
        
        allocator.deallocate(oldNodes, count);
        
        for(int i = count - 1; i < capacity - 1; ++i)  {
            nodes[i].next = i + 1;
        }
        
        nodes[capacity - 1].next = -1;
        
        next = count;
    }
    
    ++count;
    
    int n = next;
    
    next = nodes[n].next;
    
    nodes[n].children.fill(-1);

    return n;
}

int HashLife::alloc(int level) {
    assert(level > 0);
    int n = alloc();
    
    if(level > 1) {
        nodes[n][0] = alloc(level - 1);
        nodes[n][1] = alloc(level - 1);
        nodes[n][2] = alloc(level - 1);
        nodes[n][3] = alloc(level - 1);
    }else{
        nodes[n].children.fill(0);
    }
    
    nodes[n].level = level;
    
    return n;
}

void HashLife::free(int n) {
    assert(count > 0);
    
    nodes[n].next = next;
    next = n;
    --count;
}

void HashLife::trash(int n) {
    if(!nodes[n].isLeaf()) {
        trash(nodes[n][0]);
        trash(nodes[n][1]);
        trash(nodes[n][2]);
        trash(nodes[n][3]);
    }
    free(n);
}

void HashLife::copy(int dst, int src) {
    assert(nodes[dst].level == nodes[src].level);
    
    if(nodes[src].isLeaf()) {
        nodes[dst].children = nodes[src].children;
    }else{
        copy(nodes[dst][0], nodes[src][0]);
        copy(nodes[dst][1], nodes[src][1]);
        copy(nodes[dst][2], nodes[src][2]);
        copy(nodes[dst][3], nodes[src][3]);
    }
}

int HashLife::clone(int n) {
    int node = alloc();
    if(nodes[n].isLeaf()) {
        nodes[node].children = nodes[n].children;
    }else{
        nodes[node][0] = clone(nodes[n][0]);
        nodes[node][1] = clone(nodes[n][1]);
        nodes[node][2] = clone(nodes[n][2]);
        nodes[node][3] = clone(nodes[n][3]);
    }
    nodes[node].level = nodes[n].level;
    nodes[node].hash = nodes[n].hash;
    return node;
}

void HashLife::alloc_children(int n) {
    assert(nodes[n].level > 1);
    
    nodes[n][0] = alloc();
    nodes[n][1] = alloc();
    nodes[n][2] = alloc();
    nodes[n][3] = alloc();
    
    nodes[nodes[n][0]].level = nodes[n].level - 1;
    nodes[nodes[n][1]].level = nodes[n].level - 1;
    nodes[nodes[n][2]].level = nodes[n].level - 1;
    nodes[nodes[n][3]].level = nodes[n].level - 1;
}

void HashLife::expandOnce() {
    int level = nodes[root].level - 1;
    
    int n0 = ref_node(alloc(level), alloc(level), alloc(level), nodes[root][0]);
    int n1 = ref_node(alloc(level), alloc(level), nodes[root][1], alloc(level));
    int n2 = ref_node(alloc(level), nodes[root][2], alloc(level), alloc(level));
    int n3 = ref_node(nodes[root][3], alloc(level), alloc(level), alloc(level));
    
    ++nodes[root].level;
    
    nodes[root][0] = n0;
    nodes[root][1] = n1;
    nodes[root][2] = n2;
    nodes[root][3] = n3;
}
