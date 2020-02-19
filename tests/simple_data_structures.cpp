#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <iostream>
#include <vector>

#include <dynamic_array.hpp>

template<typename T>
void print(const array<T>& array) {
   for (size_t i = 0, sz = array.size(); i < sz; ++i) {
      std::cerr << array[i] << ' ';
   }
   std::cerr << '\n';
}

template <typename T>
std::vector<T> as_vector(const array<T>& array){
   std::vector<T> result(array.size());
   for (size_t i = 0, sz = array.size(); i < sz; ++i) {
      result[i] = array[i];
   }
   return result;
}

// ------------------------------------------------------------------
TEMPLATE_TEST_CASE("dynamic_arrays_commit_tests", "[dynamic_array][template]", (single_array<int>)) {
   TestType array;
   REQUIRE(array.size() == 0);

   SECTION("add values to back and delete it from back") {
      array.add_back(0);
      array.add_back(1);
      array.add_back(2);
      array.add_back(3);
      array.add_back(4);
      CHECK(array.size() == 5);
      CHECK(as_vector<int>(array) == std::vector<int>{0, 1, 2, 3, 4});

      CHECK_THROWS_WITH(array.remove(array.size()), "incorrect index requested");

      CHECK(array.remove(array.size() - 1) == 4);
      CHECK(array.remove(array.size() - 1) == 3);
      CHECK(array.remove(array.size() - 1) == 2);
      CHECK(array.remove(array.size() - 1) == 1);
      CHECK(array.remove(array.size() - 1) == 0);

      CHECK(array.size() == 0);
   }
   SECTION("add values to front and delete it from front") {
      array.add(0, 0);
      array.add(1, 0);
      array.add(2, 0);
      array.add(3, 0);
      array.add(4, 0);
      CHECK(array.size() == 5);
      CHECK(as_vector<int>(array) == std::vector<int>{4, 3, 2, 1, 0});

      CHECK(array.remove(0) == 4);
      CHECK(array.remove(0) == 3);
      CHECK(array.remove(0) == 2);
      CHECK(array.remove(0) == 1);
      CHECK(array.remove(0) == 0);

      CHECK(array.size() == 0);
   }
   SECTION("add values in middle and delete it from middle") {
      array.add(10, 0);
      array.add(20, 1);
      array.add(30, 2);
      CHECK(array.size() == 3);

      CHECK_THROWS_WITH(array.add(50, 5), "incorrect index requested");
      CHECK(array.size() == 3);

      array.add(42, 1);
      CHECK(as_vector<int>(array) == std::vector<int>{10, 42, 20, 30});
      CHECK(array.remove(1) == 42);
      CHECK(as_vector<int>(array) == std::vector<int>{10, 20, 30});
      CHECK(array.remove(1) == 20);
      CHECK(array.remove(1) == 30);
      CHECK(array.remove(0) == 10);

      CHECK(array.size() == 0);
   }
   SECTION("get values") {
      CHECK_THROWS_WITH(array.get(0), "incorrect index requested");
      array.add(10, 0);
      array.add(20, 1);
      array.add(30, 2);
      array.add(40, 3);

      CHECK(array.get(0) == 10);
      CHECK(array.get(1) == 20);
      CHECK(array.get(2) == 30);
      CHECK(array.get(3) == 40);

      CHECK_THROWS_WITH(array.get(4), "incorrect index requested");
   }
}

// ------------------------------------------------------------------
TEST_CASE("vector_array_test", "[vector_array]") {
   constexpr auto ARRAY_FACTOR{4};
   vector_array<int, ARRAY_FACTOR> array;
   REQUIRE(array.size() == 0);
   REQUIRE(array.capacity() == 0);

   SECTION("insert values without memory reallocation") {
      array.add_back(10);
      REQUIRE(array.size() == 1);
      REQUIRE(array.capacity() == ARRAY_FACTOR);
      array.add_back(20);
      array.add_back(30);
      array.add_back(40);

      REQUIRE(array.size() == ARRAY_FACTOR);
      REQUIRE(array.capacity() == ARRAY_FACTOR);
   }
   SECTION("insert values with memory reallocation") {
      array.add_back(10);
      REQUIRE(array.size() == 1);
      REQUIRE(array.capacity() == ARRAY_FACTOR);
      array.add_back(20);
      array.add_back(30);
      array.add_back(40);
      array.add_back(50);

      REQUIRE(array.size() == ARRAY_FACTOR+1);
      REQUIRE(array.capacity() == ARRAY_FACTOR*2);
   }
   SECTION("insert values with memory reallocation in the middle of array") {
      array.add_back(10);
      REQUIRE(array.size() == 1);
      REQUIRE(array.capacity() == ARRAY_FACTOR);
      array.add_back(20);
      array.add_back(30);
      array.add_back(40);
      REQUIRE(array.size() == ARRAY_FACTOR);

      array.add(42, 2);
      REQUIRE(array.size() == ARRAY_FACTOR+1);
      REQUIRE(array.capacity() == ARRAY_FACTOR*2);
      CHECK(as_vector<int>(array) == std::vector<int>{10, 20, 42, 30, 40});
   }
}