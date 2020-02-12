#include "pch.h"

#include "fib.h"
#include "pow.h"
#include "gcd.h"
#include "primes.h"

TEST(simple_pow, pow_test) {
    EXPECT_EQ(naive_pow(2, 0), 1);
    EXPECT_EQ(naive_pow(2, 1), 2);
    EXPECT_EQ(naive_pow(2, 2), 4);
    EXPECT_EQ(naive_pow(2, 10), 1'024);
    EXPECT_EQ(naive_pow(2, 16), 65'536);
    EXPECT_EQ(naive_pow(2, 32), 4'294'967'296);
}
TEST(use_pow_of_2, pow_test) {
    EXPECT_EQ(use_pow_of_2(2, 0), 1);
    EXPECT_EQ(use_pow_of_2(2, 1), 2);
    EXPECT_EQ(use_pow_of_2(2, 2), 4);
    EXPECT_EQ(use_pow_of_2(2, 10), 1'024);
    EXPECT_EQ(use_pow_of_2(2, 16), 65'536);
    EXPECT_EQ(use_pow_of_2(2, 32), 4'294'967'296);
}
TEST(use_pow_of_p, pow_test) {
    EXPECT_EQ(use_pow_of_p(2, 0), 1);
    EXPECT_EQ(use_pow_of_p(2, 1), 2);
    EXPECT_EQ(use_pow_of_p(2, 2), 4);
    EXPECT_EQ(use_pow_of_p(2, 5), 32);
    EXPECT_EQ(use_pow_of_p(2, 10), 1'024);
    EXPECT_EQ(use_pow_of_p(2, 16), 65'536);
    EXPECT_EQ(use_pow_of_p(2, 32), 4'294'967'296);
}

TEST(gcd_euclideus, gcd_test) {
    EXPECT_EQ(gcd_euclideus( 20, 15),  5);
    EXPECT_EQ(gcd_euclideus(  4,  2),  2);
    EXPECT_EQ(gcd_euclideus(  2,  2),  2);
    EXPECT_EQ(gcd_euclideus( 13, 15),  1);
    EXPECT_EQ(gcd_euclideus(121, 22), 11);
    EXPECT_EQ(gcd_euclideus(201, 15),  3);
    EXPECT_EQ(gcd_euclideus( 20,  9),  1);
    EXPECT_EQ(gcd_euclideus(125, 15),  5);
    EXPECT_EQ(gcd_euclideus(  2, 1234567890), 2);
    EXPECT_EQ(gcd_euclideus(  1, 1234567890), 1);
}

TEST(gcd_use_mod, gcd_test) {
    EXPECT_EQ(gcd_use_mod(20, 15), 5);
    EXPECT_EQ(gcd_use_mod(4, 2), 2);
    EXPECT_EQ(gcd_use_mod(2, 2), 2);
    EXPECT_EQ(gcd_use_mod(13, 15), 1);
    EXPECT_EQ(gcd_use_mod(121, 22), 11);
    EXPECT_EQ(gcd_use_mod(201, 15), 3);
    EXPECT_EQ(gcd_use_mod(20, 9), 1);
    EXPECT_EQ(gcd_use_mod(125, 15), 5);
    EXPECT_EQ(gcd_use_mod(2, 1234567890), 2);
    EXPECT_EQ(gcd_use_mod(1, 1234567890), 1);
}

TEST(gcd_use_recursive_mod, gcd_test) {
    EXPECT_EQ(gcd_use_recursive_mod(20, 15), 5);
    EXPECT_EQ(gcd_use_recursive_mod(4, 2), 2);
    EXPECT_EQ(gcd_use_recursive_mod(2, 2), 2);
    EXPECT_EQ(gcd_use_recursive_mod(13, 15), 1);
    EXPECT_EQ(gcd_use_recursive_mod(121, 22), 11);
    EXPECT_EQ(gcd_use_recursive_mod(201, 15), 3);
    EXPECT_EQ(gcd_use_recursive_mod(20, 9), 1);
    EXPECT_EQ(gcd_use_recursive_mod(125, 15), 5);
    EXPECT_EQ(gcd_use_recursive_mod(2, 1234567890), 2);
    EXPECT_EQ(gcd_use_recursive_mod(1, 1234567890), 1);
}

TEST(naive_is_prime, prime_test) {
    EXPECT_TRUE(naive_is_prime(0));
    EXPECT_TRUE(naive_is_prime(1));
    EXPECT_TRUE(naive_is_prime(2));
    EXPECT_TRUE(naive_is_prime(3));
    EXPECT_FALSE(naive_is_prime(6));
    EXPECT_TRUE(naive_is_prime(7));
    EXPECT_TRUE(naive_is_prime(11));
    EXPECT_FALSE(naive_is_prime(12));
    EXPECT_FALSE(naive_is_prime(20));
    EXPECT_TRUE(naive_is_prime(23));
    EXPECT_TRUE(naive_is_prime(29));
    EXPECT_FALSE(naive_is_prime(121));
}

TEST(is_prime, prime_test) {
    EXPECT_TRUE(is_prime(0));
    EXPECT_TRUE(is_prime(1));
    EXPECT_TRUE(is_prime(2));
    EXPECT_TRUE(is_prime(3));
    EXPECT_FALSE(is_prime(6));
    EXPECT_TRUE(is_prime(7));
    EXPECT_TRUE(is_prime(11));
    EXPECT_FALSE(is_prime(12));
    EXPECT_FALSE(is_prime(20));
    EXPECT_TRUE(is_prime(23));
    EXPECT_TRUE(is_prime(29));
    EXPECT_FALSE(is_prime(121));
}

TEST(cached_is_prime, prime_test) {
    EXPECT_TRUE(cached_is_prime(0));
    EXPECT_TRUE(cached_is_prime(1));
    EXPECT_TRUE(cached_is_prime(2));
    EXPECT_TRUE(cached_is_prime(3));
    EXPECT_FALSE(cached_is_prime(6));
    EXPECT_TRUE(cached_is_prime(7));
    EXPECT_TRUE(cached_is_prime(11));
    EXPECT_FALSE(cached_is_prime(12));
    EXPECT_FALSE(cached_is_prime(20));
    EXPECT_TRUE(cached_is_prime(23));
    EXPECT_TRUE(cached_is_prime(29));
    EXPECT_TRUE(cached_is_prime(101));
    EXPECT_FALSE(cached_is_prime(121));
}

TEST(simple_sieve_of_eratosthenes, prime_test) {
    simple_sieve_of_eratosthenes<120>();
}

TEST(cicle_fib, fib_test) {
    EXPECT_EQ(cicle_fib(0), 0);
    EXPECT_EQ(cicle_fib(1), 1);
    EXPECT_EQ(cicle_fib(2), 1);
    EXPECT_EQ(cicle_fib(3), 2);
    EXPECT_EQ(cicle_fib(4), 3);
    EXPECT_EQ(cicle_fib(5), 5);
    EXPECT_EQ(cicle_fib(6), 8);
}

TEST(recursive_fib, fib_test) {
    EXPECT_EQ(recursive_fib(0), 0);
    EXPECT_EQ(recursive_fib(1), 1);
    EXPECT_EQ(recursive_fib(2), 1);
    EXPECT_EQ(recursive_fib(3), 2);
    EXPECT_EQ(recursive_fib(4), 3);
    EXPECT_EQ(recursive_fib(5), 5);
    EXPECT_EQ(recursive_fib(6), 8);
}

TEST(golden_ratio_fib, fib_test) {
    EXPECT_EQ(golden_ratio_fib(0), 0);
    EXPECT_EQ(golden_ratio_fib(1), 1);
    EXPECT_EQ(golden_ratio_fib(2), 1);
    EXPECT_EQ(golden_ratio_fib(3), 2);
    EXPECT_EQ(golden_ratio_fib(4), 3);
    EXPECT_EQ(golden_ratio_fib(5), 5);
    EXPECT_EQ(golden_ratio_fib(6), 8);
}

TEST(compile_time_fib, fib_test) {
    EXPECT_EQ(fib<0>::value, 0);
    EXPECT_EQ(fib<1>::value, 1);
    EXPECT_EQ(fib<2>::value, 1);
    EXPECT_EQ(fib<3>::value, 2);
    EXPECT_EQ(fib<4>::value, 3);
    EXPECT_EQ(fib<5>::value, 5);
    EXPECT_EQ(fib<6>::value, 8);
}
