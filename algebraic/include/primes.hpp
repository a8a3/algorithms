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
bool sqrt_is_prime(uint32_t val) {
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
    if (val < 2) return true;

    static std::set<uint32_t> cache{2};

    if (cache.find(val) != cache.end())
        return true;

    for (const auto& i: cache) {
        if (val % i == 0) {
            return false;
        }
    }

    if (sqrt_is_prime(val)) {
        cache.insert(val);
        return true;
    }
    return false;
}

// ------------------------------------------------------------------
static uint32_t primes_count(std::function<uint32_t(uint32_t)> is_prime_func, uint32_t val) {
    uint32_t count = 0;
    for (uint32_t i = 1; i <= val; ++i) {
        if (is_prime_func(i)) {
            ++count;
        }
    }
    return count;
}

// ------------------------------------------------------------------
uint32_t naive_primes_count(uint32_t val) {
    return primes_count(naive_is_prime, val);
}

// ------------------------------------------------------------------
uint32_t cached_primes_count(uint32_t val) {
    return primes_count(cached_is_prime, val);
}

// ------------------------------------------------------------------
uint32_t sqrt_primes_count(uint32_t val) {
    return primes_count(sqrt_is_prime, val);
}

// ------------------------------------------------------------------
// time(O(n*log(log(n))), space O(n)
template<uint32_t Tval>
size_t simple_sieve_of_eratosthenes() {
    std::bitset<Tval> sieve;
    sieve.set();
    sieve.reset(0);

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

    // print
    for (size_t i = 0; i < Tval; ++i) {
        if (sieve[i])
            std::cerr << i << ", ";
    }
    std::cerr << '\n';


    return sieve.count();
}