#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <array>

#include <fib.hpp>

constexpr uint32_t fib<0>::value;
constexpr uint32_t fib<1>::value;
template<uint32_t Tn>
constexpr uint32_t fib<Tn>::value;

// ------------------------------------------------------------------
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
}

// ------------------------------------------------------------------
TEST_CASE("matrix_fib", "[fib]") {
   // fib-> 0 1 1 2 3 5 8 13
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

   std::array<uint32_t, fib_matrix::get_value(10)> arr;
   CHECK(arr.size() == 55);
}

// ------------------------------------------------------------------
TEST_CASE("use_pow_of_2", "[mul]") {
   CHECK(pow_of2_pow(10, 0) == 1);
   CHECK(pow_of2_pow(10, 1) == 10);
   CHECK(pow_of2_pow(10, 2) == 100);
   CHECK(pow_of2_pow(10, 3) == 1'000);
}