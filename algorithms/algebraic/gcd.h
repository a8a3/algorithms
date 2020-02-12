#pragma once

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