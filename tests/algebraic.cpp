#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <array>
#include <functional>
#include <iostream>
#include <thread>

#include <fib.hpp>
#include <gcd.hpp>
#include <pow.hpp>
#include <primes.hpp>

#include "files_tester.hpp"

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

// ------------------------------------------------------------------
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

// ------------------------------------------------------------------
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
   CHECK(fib<50>::value == 12'586'269'025);
}

// ------------------------------------------------------------------
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
class fib_files_test {
   friend std::istream& operator>> (std::istream&, fib_files_test&);
   uint32_t value_{0};

public:
   static std::string get_test_dir() {
      return fs::current_path().parent_path() / "tests/data/algebraic/fib";
   }

   bool execute(const std::string& expected) const {
      using executor = std::pair<std::function<uint64_t(uint8_t)>, const char*>;
      std::vector<executor> executors{{recursive_fib,          "recursive fib"},
                                      {cycle_fib,              "cycle fib"},
                                      {golden_ratio_fib,       "golden ratio fib"},
                                      {&fib_matrix::get_value, "matrix fib"} };

      for (const auto& e: executors) {
         uint64_t result{0};
         {
            std::cerr << "test: "  << e.second << ", value: " << value_ << ", expected: " << expected << '\n';
            timer t;
            result = e.first(value_);
         }
         if (expected != std::to_string(result)) {
            std::cerr << "test: " << e.second << " is failed! expected: " << expected << ", result: " << result << '\n';
            return false;
         }
      }
      return true;
   }
};

std::istream& operator>>(std::istream& is, fib_files_test& test) {
   std::string value;
   is >> value;
   test.value_ = std::stoi(value);
   return is;
}

// ------------------------------------------------------------------
TEST_CASE("fib_files_test", "[fib]") {
   files_tester::check<fib_files_test>();
//   test: recursive fib, value: 50, expected: 12586269025
//   elapsed time: 49779631 us
//   test: cycle fib, value: 50, expected: 12586269025
//   elapsed time: 0 us
//   test: golden ratio fib, value: 50, expected: 12586269025
//   elapsed time: 12 us
//   test: matrix fib, value: 50, expected: 12586269025
//   elapsed time: 0 us
//   test: recursive fib, value: 10, expected: 55
//   elapsed time: 0 us
//   test: cycle fib, value: 10, expected: 55
//   elapsed time: 0 us
//   test: golden ratio fib, value: 10, expected: 55
//   elapsed time: 0 us
//   test: matrix fib, value: 10, expected: 55
//   elapsed time: 0 us
//   test: recursive fib, value: 20, expected: 6765
//   elapsed time: 23 us
//   test: cycle fib, value: 20, expected: 6765
//   elapsed time: 0 us
//   test: golden ratio fib, value: 20, expected: 6765
//   elapsed time: 0 us
//   test: matrix fib, value: 20, expected: 6765
//   elapsed time: 0 us
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

// ------------------------------------------------------------------
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

// ------------------------------------------------------------------
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
class gcd_files_test {
   friend std::istream& operator>> (std::istream&, gcd_files_test&);
   uint32_t first_ {0};
   uint32_t second_{0};

   struct executor {
      std::function<uint32_t(uint32_t, uint32_t)> func_;
      const char*                                 description_;
   };

public:
   static std::string get_test_dir() {
      return fs::current_path().parent_path() / "tests/data/algebraic/gcd";
   }

   bool execute(const std::string& expected) const {
      std::vector<executor> executors{{gcd_euclideus,         "gcd_euclideus"},
                                      {gcd_use_mod,           "gcd_use_mod"},
                                      {gcd_use_recursive_mod, "gcd_use_recursive_mod"} };

      for (const auto& e: executors) {
         uint64_t result{0};
         {
            std::cerr << "test: "  << e.description_ << ", first: " << first_ << ", second: " << second_ << ", expected: " << expected << '\n';
            timer t;
            result = e.func_(first_, second_);
         }
         if (expected != std::to_string(result)) {
            std::cerr << "test: " << e.description_ << " is failed! expected: " << expected << ", result: " << result << '\n';
            return false;
         }
      }
      return true;
   }
};

std::istream& operator>>(std::istream& is, gcd_files_test& test) {
   std::string first, second;
   is >> first >> second;
   test.first_ = std::stoi(first);
   test.second_ = std::stoi(second);
   return is;
}

// ------------------------------------------------------------------
TEST_CASE("gcd_files_test", "[gcd]") {
   files_tester::check<gcd_files_test>();

//   test: gcd_euclideus, first: 2, second: 1234567890, expected: 2
//   elapsed time: 1120044 us
//   test: gcd_use_mod, first: 2, second: 1234567890, expected: 2
//   elapsed time: 0 us
//   test: gcd_use_recursive_mod, first: 2, second: 1234567890, expected: 2
//   elapsed time: 0 us
//   test: gcd_euclideus, first: 201, second: 15, expected: 3
//   elapsed time: 0 us
//   test: gcd_use_mod, first: 201, second: 15, expected: 3
//   elapsed time: 0 us
//   test: gcd_use_recursive_mod, first: 201, second: 15, expected: 3
//   elapsed time: 0 us
//   test: gcd_euclideus, first: 20, second: 15, expected: 5
//   elapsed time: 0 us
//   test: gcd_use_mod, first: 20, second: 15, expected: 5
//   elapsed time: 0 us
//   test: gcd_use_recursive_mod, first: 20, second: 15, expected: 5
//   elapsed time: 0 us
//   test: gcd_euclideus, first: 1, second: 1234567890, expected: 1
//   elapsed time: 1274348 us
//   test: gcd_use_mod, first: 1, second: 1234567890, expected: 1
//   elapsed time: 0 us
//   test: gcd_use_recursive_mod, first: 1, second: 1234567890, expected: 1
//   elapsed time: 0 us
//   test: gcd_euclideus, first: 121, second: 22, expected: 11
//   elapsed time: 0 us
//   test: gcd_use_mod, first: 121, second: 22, expected: 11
//   elapsed time: 0 us
//   test: gcd_use_recursive_mod, first: 121, second: 22, expected: 11
//   elapsed time: 0 us
}

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
   CHECK(use_pow_of_p(2,  0) == 1);
   CHECK(use_pow_of_p(2,  1) == 2);
   CHECK(use_pow_of_p(2,  2) == 4);
   CHECK(use_pow_of_p(2,  5) == 32);
   CHECK(use_pow_of_p(2, 10) == 1'024);
   CHECK(use_pow_of_p(2, 16) == 65'536);
   CHECK(use_pow_of_p(2, 32) == 4'294'967'296);
}

// ------------------------------------------------------------------
class pow_files_test {
   friend std::istream& operator>> (std::istream&, pow_files_test&);
   uint64_t base_{0};
   uint32_t pow_{0};

   struct executor {
      std::function<uint64_t(uint64_t, uint32_t)> func_;
      const char*                                 description_;
   };

public:
   static std::string get_test_dir() {
      return fs::current_path().parent_path() / "tests/data/algebraic/pow";
   }

   bool execute(const std::string& expected) const {
      std::vector<executor> executors{{naive_pow,    "simple_pow"},
                                      {use_pow_of_2, "use_pow_of_2"},
                                      {use_pow_of_p, "use_pow_of_p"} };

      for (const auto& e: executors) {
         uint64_t result{0};
         {
            std::cerr << "test: "  << e.description_ << ", base: " << base_ << ", pow: " << pow_ << ", expected: " << expected << '\n';
            timer t;
            result = e.func_(base_, pow_);
         }
         if (expected != std::to_string(result)) {
            std::cerr << "test: " << e.description_ << " is failed! expected: " << expected << ", result: " << result << '\n';
            return false;
         }
      }
      return true;
   }
};

std::istream& operator>>(std::istream& is, pow_files_test& test) {
   std::string base, pow;
   is >> base >> pow;
   test.base_ = std::stoi(base);
   test.pow_ = std::stoi(pow);
   return is;
}


// ------------------------------------------------------------------
TEST_CASE("pow_files_test", "[pow]") {
   files_tester::check<pow_files_test>();
//   test: simple_pow, base: 2, pow: 16, expected: 65536
//   elapsed time: 0 us
//   test: use_pow_of_2, base: 2, pow: 16, expected: 65536
//   elapsed time: 0 us
//   test: use_pow_of_p, base: 2, pow: 16, expected: 65536
//   elapsed time: 0 us
//   test: simple_pow, base: 2, pow: 10, expected: 1024
//   elapsed time: 0 us
//   test: use_pow_of_2, base: 2, pow: 10, expected: 1024
//   elapsed time: 0 us
//   test: use_pow_of_p, base: 2, pow: 10, expected: 1024
//   elapsed time: 0 us
//   test: simple_pow, base: 2, pow: 1, expected: 2
//   elapsed time: 0 us
//   test: use_pow_of_2, base: 2, pow: 1, expected: 2
//   elapsed time: 0 us
//   test: use_pow_of_p, base: 2, pow: 1, expected: 2
//   elapsed time: 0 us
//   test: simple_pow, base: 2, pow: 32, expected: 4294967296
//   elapsed time: 0 us
//   test: use_pow_of_2, base: 2, pow: 32, expected: 4294967296
//   elapsed time: 0 us
//   test: use_pow_of_p, base: 2, pow: 32, expected: 4294967296
//   elapsed time: 0 us
//   test: simple_pow, base: 10, pow: 4, expected: 10000
//   elapsed time: 0 us
//   test: use_pow_of_2, base: 10, pow: 4, expected: 10000
//   elapsed time: 0 us
//   test: use_pow_of_p, base: 10, pow: 4, expected: 10000
//   elapsed time: 0 us
}


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

// ------------------------------------------------------------------
class primes_files_test {
   friend std::istream& operator>> (std::istream&, primes_files_test&);
   uint64_t num_{0};

   struct executor {
      std::function<uint32_t(uint32_t)> func_;
      const char*                       description_;
   };

public:
   static std::string get_test_dir() {
      return fs::current_path().parent_path() / "tests/data/algebraic/primes";
   }

   bool execute(const std::string& expected) const {
      std::vector<executor> executors{{naive_primes_count,  "naive_primes_count"},
                                      {sqrt_primes_count,   "sqrt_primes_count"},
                                      {cached_primes_count, "cached_primes_count"} };

      for (const auto& e: executors) {
         uint32_t result{0};
         {
            std::cerr << "test: "  << e.description_ << ", number: " << num_ << ", expected: " << expected << '\n';
            timer t;
            result = e.func_(num_);
         }
         if (expected != std::to_string(result)) {
            std::cerr << "test: " << e.description_ << " is failed! expected: " << expected << ", result: " << result << '\n';
            return false;
         }
      }
      return true;
   }
};

std::istream& operator>>(std::istream& is, primes_files_test& test) {
   std::string number;
   is >> number;
   test.num_ = std::stoi(number);
   return is;
}

TEST_CASE("primes_files_test", "[primes_count]") {
   files_tester::check<primes_files_test>();

//   test: naive_primes_count, number: 50000, expected: 5134
//   elapsed time: 453347 us
//   test: sqrt_primes_count, number: 50000, expected: 5134
//   elapsed time: 3806 us
//   test: cached_primes_count, number: 50000, expected: 5134
//   elapsed time: 197377 us
//   test: naive_primes_count, number: 10000, expected: 1230
//   elapsed time: 34079 us
//   test: sqrt_primes_count, number: 10000, expected: 1230
//   elapsed time: 366 us
//   test: cached_primes_count, number: 10000, expected: 1230
//   elapsed time: 2364 us
//   test: naive_primes_count, number: 100, expected: 26
//   elapsed time: 4 us
//   test: sqrt_primes_count, number: 100, expected: 26
//   elapsed time: 1 us
//   test: cached_primes_count, number: 100, expected: 26
//   elapsed time: 5 us
//   test: naive_primes_count, number: 100000, expected: 9593
//   elapsed time: 1553549 us
//   test: sqrt_primes_count, number: 100000, expected: 9593
//   elapsed time: 6479 us
//   test: cached_primes_count, number: 100000, expected: 9593
//   elapsed time: 192075 us
//   test: naive_primes_count, number: 1000, expected: 169
//   elapsed time: 176 us
//   test: sqrt_primes_count, number: 1000, expected: 169
//   elapsed time: 17 us
//   test: cached_primes_count, number: 1000, expected: 169
//   elapsed time: 48 us
}