//
//  LifeRule.h
//  HashLife
//
//  Created by Arthur Sun on 4/17/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef LifeRule_h
#define LifeRule_h

#include <array>

struct LifeRule
{
    std::array<std::array<char, 9>, 2> rules;
    
    const std::array<char, 9>& operator [] (uint8_t i) const
    {
        return rules[i];
    }
};

#endif /* LifeRule_h */
