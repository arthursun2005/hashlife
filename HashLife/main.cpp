//
//  main.cpp
//  HashLife
//
//  Created by Arthur Sun on 4/16/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include <iostream>
#include "HashLife.hpp"

HashLife life;

std::vector<unsigned long> clocks;

#define mark clocks.push_back(clock())

float ms(int i) {
    return 1000.0f / (float) CLOCKS_PER_SEC * (clocks[i + 1] - clocks[i]);
}

void printAll() {
    for(int i = 0; i < clocks.size() - 1; ++i)
        printf("%.5f\n", ms(i));
}

int main(int argc, const char * argv[]) {
    mark;
    
    int n = 1;
    
    for(int i = 0; i < n; ++i) {
        life.step();
    }
    
    mark;
    
    printf("%.5f\n", ms(0));
    
    return 0;
}
