#pragma once

#include <cstdint>

// ------------------------------------------------------------------
// time O(n), space O(1)
uint32_t gcd_euclideus(uint32_t a, uint32_t b) {
    while (a != b) {
        if (a > b) {
            a = a - b;
        } else {
            b = b - a;
        }
    }
    return a;
}

// ------------------------------------------------------------------
// time O(n), space O(1)
uint32_t gcd_use_mod(uint32_t a, uint32_t b) {
    while (a != b) {
        if (a > b) {
            a = a % b;
            if (a == 0) return b;
        }
        else {
            b = b % a;
            if (b == 0) return a;
        }
    }
    return a;
}

// ------------------------------------------------------------------
// time O(log^2(N)), space O(1)
uint32_t gcd_use_recursive_mod(uint32_t a, uint32_t b) {
    return b == 0 ? a 
                  : gcd_use_recursive_mod(b, a % b);
}

bool is_odd (uint32_t v) {return v & 1;}
bool is_even(uint32_t v) {return !is_odd(v);}

// ------------------------------------------------------------------
uint32_t steins_algorithm(uint32_t a, uint32_t b) {
    if (a == b) return a;
    if (a == 0) return b;
    if (b == 0) return a;

    if (is_even(a) && is_even(b)) return steins_algorithm(a>>1, b>>1) << 1;
    if (is_even(a) && is_odd (b)) return steins_algorithm(a>>1, b);
    if (is_odd (a) && is_even(b)) return steins_algorithm(a, b >> 1);

    if (a > b) {
        return steins_algorithm((a - b) >> 1, b);
    }
    return steins_algorithm(a, (b-a) >> 1);
}