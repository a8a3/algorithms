#pragma once

#include <cstdint>

// ------------------------------------------------------------------
// time O(N), space O(1)
uint64_t naive_pow(uint64_t val, uint32_t p) {
    if (p <= 0) return 1;
    if (p == 1) return val;
    
    uint64_t res{val};
    
    while (--p > 0) {
        res *= val;
    }
    return res;
}

// ------------------------------------------------------------------
// time O(N), space O(1)
uint64_t use_pow_of_2(uint64_t val, uint32_t p) {
    if (p <= 0) return 1;
    if (p == 1) return val;
    
    uint32_t current_pow = 2;
    uint64_t result{val*val};
 
    while (true) {
        if (current_pow + current_pow <= p) {
            result *= result;
            current_pow += current_pow;
        }
        else {
            while (current_pow < p) {
                result *= val;
                ++current_pow;
            }
            break;
        }
    }
    return result;
}

// ------------------------------------------------------------------
// time O(logN), space O(1)
uint64_t use_pow_of_p(uint64_t base, uint32_t p) {
    if (p <= 0) return 1;
    if (p == 1) return base;

    uint64_t result{1};
    do {
        if (p & 1) {
            result *= base;
        }
        base *= base;
        p >>= 1;
    } while (p > 0);

    return result;
}