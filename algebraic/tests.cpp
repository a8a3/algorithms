#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <array>
#include <functional>
#include <iostream>
#include <thread>

#include <fib.hpp>
#include <gcd.hpp>
#include <pow.hpp>
#include <primes.hpp>

constexpr uint64_t fib<0>::value;
constexpr uint64_t fib<1>::value;
template<uint8_t Tn>
constexpr uint64_t fib<Tn>::value;

// ------------------------------------------------------------------
TEST_CASE("cicle_fib", "[fib]") {
   CHECK(cycle_fib(0) == 0);
   CHECK(cycle_fib(1) == 1);
   CHECK(cycle_fib(2) == 1);
   CHECK(cycle_fib(3) == 2);
   CHECK(cycle_fib(4) == 3);
   CHECK(cycle_fib(5) == 5);
   CHECK(cycle_fib(6) == 8);
}
TEST_CASE("recursive_fib", "[fib]") {
   CHECK(recursive_fib(0) == 0);
   CHECK(recursive_fib(1) == 1);
   CHECK(recursive_fib(2) == 1);
   CHECK(recursive_fib(3) == 2);
   CHECK(recursive_fib(4) == 3);
   CHECK(recursive_fib(5) == 5);
   CHECK(recursive_fib(6) == 8);
   CHECK(recursive_fib(20) == 6'765);
}
TEST_CASE("golden_ratio_fib", "[fib]") {
   CHECK(golden_ratio_fib(0) == 0);
   CHECK(golden_ratio_fib(1) ==  1);
   CHECK(golden_ratio_fib(2) == 1);
   CHECK(golden_ratio_fib(3) == 2);
   CHECK(golden_ratio_fib(4) == 3);
   CHECK(golden_ratio_fib(5) == 5);
   CHECK(golden_ratio_fib(6) == 8);
   CHECK(golden_ratio_fib(50) == 12'586'269'025);
}
TEST_CASE("meta_fib", "[fib]") {
   CHECK(fib<0>::value == 0);
   CHECK(fib<1>::value == 1);
   CHECK(fib<2>::value == 1);
   CHECK(fib<3>::value == 2);
   CHECK(fib<4>::value == 3);
   CHECK(fib<5>::value == 5);
   CHECK(fib<6>::value == 8);
   CHECK(fib<7>::value == 13);
   CHECK(fib<8>::value == 21);
   CHECK(fib<9>::value == 34);
   CHECK(fib<50>::value == 12'586'269'025);
}
TEST_CASE("matrix_fib", "[fib]") {
   CHECK(fib_matrix::get_value(0) == 0);
   CHECK(fib_matrix::get_value(1) == 1);
   CHECK(fib_matrix::get_value(2) == 1);
   CHECK(fib_matrix::get_value(3) == 2);
   CHECK(fib_matrix::get_value(4) == 3);
   CHECK(fib_matrix::get_value(5) == 5);
   CHECK(fib_matrix::get_value(6) == 8);
   CHECK(fib_matrix::get_value(7) == 13);
   CHECK(fib_matrix::get_value(8) == 21);
   CHECK(fib_matrix::get_value(9) == 34);
   CHECK(fib_matrix::get_value(10) == 55);
   CHECK(fib_matrix::get_value(11) == 89);
   CHECK(fib_matrix::get_value(12) == 144);
   CHECK(fib_matrix::get_value(20) == 6'765);
   CHECK(fib_matrix::get_value(50) == 12'586'269'025);

   std::array<uint32_t, fib_matrix::get_value(10)> arr;
   CHECK(arr.size() == 55);

   std::cerr << "fib 30: " << fib_matrix::get_value(30) << '\n';
}

// ------------------------------------------------------------------
TEST_CASE("gcd_euclideus", "[gcd]") {
   CHECK(gcd_euclideus( 20, 15) ==  5);
   CHECK(gcd_euclideus(  4,  2) ==  2);
   CHECK(gcd_euclideus(  2,  2) ==  2);
   CHECK(gcd_euclideus( 13, 15) ==  1);
   CHECK(gcd_euclideus(121, 22) == 11);
   CHECK(gcd_euclideus(201, 15) ==  3);
   CHECK(gcd_euclideus( 20,  9) ==  1);
   CHECK(gcd_euclideus(125, 15) ==  5);
   CHECK(gcd_euclideus(  2, 1234567890) == 2);
   CHECK(gcd_euclideus(  1, 1234567890) == 1);
}
TEST_CASE("gcd_use_mod", "[gcd]") {
   CHECK(gcd_use_mod(20, 15)  == 5);
   CHECK(gcd_use_mod(4, 2)    == 2);
   CHECK(gcd_use_mod(2, 2)    == 2);
   CHECK(gcd_use_mod(13, 15)  == 1);
   CHECK(gcd_use_mod(121, 22) == 11);
   CHECK(gcd_use_mod(201, 15) == 3);
   CHECK(gcd_use_mod(20, 9)   == 1);
   CHECK(gcd_use_mod(125, 15) == 5);
   CHECK(gcd_use_mod(2, 1'234'567'890) == 2);
   CHECK(gcd_use_mod(1, 1'234'567'890) == 1);
}
TEST_CASE("gcd_use_recursive_mod", "[gcd]") {
   CHECK(gcd_use_recursive_mod(20,  15) ==  5);
   CHECK(gcd_use_recursive_mod(4,    2) ==  2);
   CHECK(gcd_use_recursive_mod(2,    2) ==  2);
   CHECK(gcd_use_recursive_mod(13,  15) ==  1);
   CHECK(gcd_use_recursive_mod(121, 22) ==  11);
   CHECK(gcd_use_recursive_mod(201, 15) ==  3);
   CHECK(gcd_use_recursive_mod(20,   9) ==  1);
   CHECK(gcd_use_recursive_mod(125, 15) ==  5);
   CHECK(gcd_use_recursive_mod(2, 1'234'567'890) == 2);
   CHECK(gcd_use_recursive_mod(1, 1'234'567'890) == 1);
}

// ------------------------------------------------------------------
TEST_CASE("simple_pow", "[pow]") {
   CHECK(naive_pow(2,  0) == 1);
   CHECK(naive_pow(2,  1) == 2);
   CHECK(naive_pow(2,  2) == 4);
   CHECK(naive_pow(2, 10) == 1'024);
   CHECK(naive_pow(2, 16) == 65'536);
   CHECK(naive_pow(2, 32) == 4'294'967'296);
}
TEST_CASE("use_pow_of_2", "[pow]") {
   CHECK(use_pow_of_2(2,  0) == 1);
   CHECK(use_pow_of_2(2,  1) == 2);
   CHECK(use_pow_of_2(2,  2) == 4);
   CHECK(use_pow_of_2(2, 10) == 1'024);
   CHECK(use_pow_of_2(2, 16) == 65'536);
   CHECK(use_pow_of_2(2, 32) == 4'294'967'296);
}
TEST_CASE("use_pow_of_p", "[pow]") {
   CHECK(use_pow_of_p(2,  0) == 1.);
   CHECK(use_pow_of_p(2,  1) == 2.);
   CHECK(use_pow_of_p(2,  2) == 4.);
   CHECK(use_pow_of_p(2,  5) == 32.);
   CHECK(use_pow_of_p(2, 10) == 1'024.);
   CHECK(use_pow_of_p(2, 16) == 65'536.);
   CHECK(use_pow_of_p(2, 32) == 4'294'967'296.);
}

// ------------------------------------------------------------------
TEST_CASE("naive_is_prime", "[primes]") {
   CHECK(naive_is_prime(0));
   CHECK(naive_is_prime(1));
   CHECK(naive_is_prime(2));
   CHECK(naive_is_prime(3));
   CHECK_FALSE(naive_is_prime(6));
   CHECK(naive_is_prime(7));
   CHECK(naive_is_prime(11));
   CHECK_FALSE(naive_is_prime(12));
   CHECK_FALSE(naive_is_prime(20));
   CHECK(naive_is_prime(23));
   CHECK(naive_is_prime(29));
   CHECK_FALSE(naive_is_prime(121));
}
TEST_CASE("mod_is_prime", "[primes]") {
   CHECK(sqrt_is_prime(0));
   CHECK(sqrt_is_prime(1));
   CHECK(sqrt_is_prime(2));
   CHECK(sqrt_is_prime(3));
   CHECK_FALSE(sqrt_is_prime(6));
   CHECK(sqrt_is_prime(7));
   CHECK(sqrt_is_prime(11));
   CHECK_FALSE(sqrt_is_prime(12));
   CHECK_FALSE(sqrt_is_prime(20));
   CHECK(sqrt_is_prime(23));
   CHECK(sqrt_is_prime(29));
   CHECK_FALSE(sqrt_is_prime(121));
}
TEST_CASE("cached_is_prime", "[primes]") {
   CHECK(cached_is_prime(0));
   CHECK(cached_is_prime(1));
   CHECK(cached_is_prime(2));
   CHECK(cached_is_prime(3));
   CHECK_FALSE(cached_is_prime(6));
   CHECK(cached_is_prime(7));
   CHECK(cached_is_prime(11));
   CHECK_FALSE(cached_is_prime(12));
   CHECK_FALSE(cached_is_prime(20));
   CHECK(cached_is_prime(23));
   CHECK(cached_is_prime(29));
   CHECK(cached_is_prime(101));
   CHECK_FALSE(cached_is_prime(121));
}
// ------------------------------------------------------------------
TEST_CASE("naive_primes_count", "[primes_count]") {
   CHECK(naive_primes_count(100) == 26);
   CHECK(naive_primes_count(1'000) == 169);
   CHECK(naive_primes_count(10'000) == 1'230);
   CHECK(naive_primes_count(50'000) == 5'134);
   CHECK(naive_primes_count(100'000) == 9'593);
}
TEST_CASE("mod_primes_count", "[primes_count]") {
   CHECK(sqrt_primes_count(100) == 26);
   CHECK(sqrt_primes_count(1'000) == 169);
   CHECK(sqrt_primes_count(10'000) == 1'230);
   CHECK(sqrt_primes_count(50'000) == 5'134);
   CHECK(sqrt_primes_count(100'000) == 9'593);
}
TEST_CASE("cached_primes_count", "[primes_count]") {
   CHECK(cached_primes_count(100) == 26);
   CHECK(cached_primes_count(1'000) == 169);
   CHECK(cached_primes_count(10'000) == 1'230);
   CHECK(cached_primes_count(50'000) == 5'134);
   CHECK(cached_primes_count(100'000) == 9'593);
}
