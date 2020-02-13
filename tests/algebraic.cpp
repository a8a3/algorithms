#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <array>
#include <functional>
#include <iostream>
#include <thread>

#include <fib.hpp>
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
         const auto str_result = std::to_string(result);
         if (expected != str_result) {
            std::cerr << "test: " << e.second << " is failed! expected: " << expected << ", result: " << str_result << '\n';
            return false;
         }
//       std::this_thread::sleep_for(std::chrono::seconds{1});
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
TEST_CASE("fib", "[files_test]") {
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