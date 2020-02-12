#pragma once

#include <bitset>
#include <set>

// ------------------------------------------------------------------
// time O(n), space O(1)
bool naive_is_prime(uint32_t val) {
    if (val < 4) return true;
    uint32_t delimeter{2};

    while (delimeter < val) {
        if (val % delimeter == 0) return false;
        ++delimeter;
    }
    return true;
}

// ------------------------------------------------------------------
// time O(n), space O(1)
bool is_prime(uint32_t val) {
    if (val < 4) return true;
    uint32_t delimeter{ 2 };

    while (delimeter * delimeter <= val) {
        if (val % delimeter == 0) return false;
        ++delimeter;
    }
    return true;
}

// ------------------------------------------------------------------
// time O(log(n)), space O(log(n))
bool cached_is_prime(uint32_t val) {

    if (val < 3) return true;
    static std::set<uint32_t> cache{2};

    for (const auto& i: cache) {
        if (i * i > val) {
            break; // for
        }
        if (val % i == 0) {
            return false;
        }
    }
    cache.insert(val);
    return true;
}

// ------------------------------------------------------------------
// time O(n*m), space O(1)
uint32_t primes_count(uint32_t val) {
    uint32_t count = 0;
    for (uint32_t i = 1; i < val; ++i) {
        if (naive_is_prime(i)) {
            ++count;
        }
    }
    return count;
}

// ------------------------------------------------------------------
// time(O(n*log(log(n))), space O(n)
template<uint32_t Tval>
void simple_sieve_of_eratosthenes() {
    std::bitset<Tval> sieve;
    sieve.set();
    
    const auto mark_childs_as_non_primes = [&sieve](size_t prime) {
        uint32_t ind = prime * prime;

        while (ind < Tval) {
            sieve.reset(ind);
            ind += prime;
        }
    };

    for (size_t i = 2; i < Tval; ++i) {
        if (sieve[i]) {
            mark_childs_as_non_primes(i);
        }
    }
}