#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

#include <dynamic_array.hpp>
#include <heap.hpp>

template<typename T>
void print(const T& array) {
   for (size_t i = 0, sz = array.size(); i < sz; ++i) {
      std::cerr << array[i] << ' ';
   }
   std::cerr << '\n';
}

template <typename T, class A>
std::vector<T> as_vector(const A& array){
   std::vector<T> result(array.size());
   for (size_t i = 0, sz = array.size(); i < sz; ++i) {
      result[i] = array.get(i);
   }
   return result;
}

// ------------------------------------------------------------------
TEMPLATE_TEST_CASE("dynamic_arrays_common_tests", "[dynamic_array][template]", (single_array<int>), (vector_array<int, 5>), (factor_array<int, 5>), (matrix_array<int, 5>)) {
   TestType array;
   REQUIRE(array.size() == 0);

   SECTION("add values to back and remove it from back") {
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
   SECTION("add values to front and remove it from front") {
      array.add_front(0);
      array.add_front(1);
      array.add_front(2);
      array.add_front(3);
      array.add_front(4);
      CHECK(array.size() == 5);
      CHECK(as_vector<int>(array) == std::vector<int>{4, 3, 2, 1, 0});

      CHECK(array.remove(0) == 4);
      CHECK(array.remove(0) == 3);
      CHECK(array.remove(0) == 2);
      CHECK(array.remove(0) == 1);
      CHECK(array.remove(0) == 0);

      CHECK(array.size() == 0);
   }
   SECTION("add values in middle and remove it from middle") {
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
   SECTION("access by idx") {
      array.add(10, 0);
      array.add(20, 1);
      array.add(30, 2);

      CHECK(array[0] == 10);
      CHECK(array[1] == 20);
      CHECK(array[2] == 30);

      array[0] = 100;
      array[1] = 200;
      array[2] = 300;

      CHECK(array[0] == 100);
      CHECK(array[1] == 200);
      CHECK(array[2] == 300);
   }
}

// ------------------------------------------------------------------
TEST_CASE("vector_array_test", "[vector_array]") {
   constexpr auto FACTOR{4};
   vector_array<int, FACTOR> array;
   REQUIRE(array.size() == 0);
   REQUIRE(array.capacity() == FACTOR);

   SECTION("insert values without memory reallocation") {
      array.add_back(10);
      REQUIRE(array.size() == 1);
      REQUIRE(array.capacity() == FACTOR);
      array.add_back(20);
      array.add_back(30);
      array.add_back(40);

      REQUIRE(array.size() == FACTOR);
      REQUIRE(array.capacity() == FACTOR);
   }
   SECTION("insert values with memory reallocation") {
      array.add_back(10);
      REQUIRE(array.size() == 1);
      REQUIRE(array.capacity() == FACTOR);
      array.add_back(20);
      array.add_back(30);
      array.add_back(40);
      array.add_back(50);

      REQUIRE(array.size() == FACTOR+1);
      REQUIRE(array.capacity() == FACTOR+FACTOR);
   }
   SECTION("insert values with memory reallocation in the middle of array") {
      array.add_back(10);
      REQUIRE(array.size() == 1);
      REQUIRE(array.capacity() == FACTOR);
      array.add_back(20);
      array.add_back(30);
      array.add_back(40);
      REQUIRE(array.size() == FACTOR);

      array.add(42, 2);
      REQUIRE(array.size() == FACTOR+1);
      REQUIRE(array.capacity() == FACTOR+FACTOR);
      CHECK(as_vector<int>(array) == std::vector<int>{10, 20, 42, 30, 40});
   }
}

// ------------------------------------------------------------------
TEST_CASE("factor_array_test", "[factor_array]") {
    constexpr auto FACTOR{ 2 };
    factor_array<int, FACTOR> array;
    REQUIRE(array.size() == 0);
    REQUIRE(array.capacity() == FACTOR);

    SECTION("insert values without memory reallocation") {
        array.add_back(10);
        REQUIRE(array.size() == 1);
        REQUIRE(array.capacity() == FACTOR);
        array.add_back(20);

        REQUIRE(array.size() == FACTOR);
        REQUIRE(array.capacity() == FACTOR);
    }
    SECTION("insert values with memory reallocation") {
        array.add_back(10);
        REQUIRE(array.size() == 1);
        REQUIRE(array.capacity() == FACTOR);
        array.add_back(20);
        array.add_back(30);

        REQUIRE(array.size() == FACTOR + 1);
        REQUIRE(array.capacity() == FACTOR * FACTOR);
    }
}

// ------------------------------------------------------------------
TEST_CASE("matrix_array_test", "[matrix_array]") {
   constexpr auto FACTOR{ 3 };
   matrix_array<int, FACTOR> array;
   REQUIRE(array.size() == 0);
   REQUIRE(array.capacity() == FACTOR);

   SECTION("insert values to front without memory reallocation") {
      array.add_front(10);
      REQUIRE(array.size() == 1);
      REQUIRE(array.capacity() == FACTOR);
      array.add_front(20);
      array.add_front(30);

      REQUIRE(array.size() == FACTOR);
      REQUIRE(array.capacity() == FACTOR);

      CHECK(as_vector<int>(array) == std::vector<int>{30, 20, 10});
   }
   SECTION("insert values to front with memory reallocation") {
      array.add_front(10);
      array.add_front(20);
      array.add_front(30);
      array.add_front(40);

      REQUIRE(array.size() == FACTOR + 1);
      REQUIRE(array.capacity() == FACTOR + FACTOR);

      CHECK(as_vector<int>(array) == std::vector<int>{40, 30, 20, 10});
   }
   SECTION("insert values to back without memory reallocation") {
      array.add_back(10);
      REQUIRE(array.size() == 1);
      REQUIRE(array.capacity() == FACTOR);
      array.add_back(20);
      array.add_back(30);

      REQUIRE(array.size() == FACTOR);
      REQUIRE(array.capacity() == FACTOR);

      CHECK(as_vector<int>(array) == std::vector<int>{10, 20, 30});
   }
   SECTION("insert values to back with memory reallocation") {
      array.add_back(10);
      array.add_back(20);
      array.add_back(30);
      array.add_back(40);

      REQUIRE(array.size() == FACTOR + 1);
      REQUIRE(array.capacity() == FACTOR + FACTOR);

      CHECK(as_vector<int>(array) == std::vector<int>{10, 20, 30, 40});
   }
   SECTION("insert values in the middle of array without memory reallocation") {
      array.add_back(10);
      array.add_back(20);
      CHECK(as_vector<int>(array) == std::vector<int>{10, 20});

      array.add(42, 1);
      CHECK(as_vector<int>(array) == std::vector<int>{10, 42, 20});
   }
   SECTION("insert values in the middle of array with memory reallocation") {
       array.add_back(1);
       array.add_back(2);
       array.add_back(3);
       array.add_back(4);
       array.add_back(5);
       array.add_back(6);
       array.add_back(7);
       CHECK(as_vector<int>(array) == std::vector<int>{1, 2, 3, 4, 5, 6, 7});

       array.add(42, 1);
       CHECK(as_vector<int>(array) == std::vector<int>{1, 42, 2, 3, 4, 5, 6, 7});
   }
   SECTION("remove values from back") {
      array.add_back(1);
      array.add_back(2);
      array.add_back(3);
      array.add_back(4);
      array.add_back(5);
      array.add_back(6);
      array.add_back(7);
      CHECK(as_vector<int>(array) == std::vector<int>{1, 2, 3, 4, 5, 6, 7});
      CHECK(array.size() == 7);

      CHECK(array.remove(6) == 7);
      CHECK(array.remove(5) == 6);
      CHECK(array.remove(4) == 5);
      CHECK(array.remove(3) == 4);
      CHECK(array.remove(2) == 3);
      CHECK(array.remove(1) == 2);
      CHECK(array.remove(0) == 1);

      CHECK(array.size() == 0);
   }
   SECTION("remove values from front") {
      array.add_back(1);
      array.add_back(2);
      array.add_back(3);
      array.add_back(4);
      array.add_back(5);
      array.add_back(6);
      array.add_back(7);
      array.add_back(8);
      REQUIRE(as_vector<int>(array) == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8});
      CHECK(array.size() == 8);

      CHECK(array.remove(0) == 1);
      REQUIRE(as_vector<int>(array) == std::vector<int>{2, 3, 4, 5, 6, 7, 8});

      CHECK(array.remove(0) == 2);
      REQUIRE(as_vector<int>(array) == std::vector<int>{3, 4, 5, 6, 7, 8});

      CHECK(array.remove(0) == 3);
      REQUIRE(as_vector<int>(array) == std::vector<int>{4, 5, 6, 7, 8});

      CHECK(array.remove(0) == 4);
      REQUIRE(as_vector<int>(array) == std::vector<int>{5, 6, 7, 8});

      CHECK(array.remove(0) == 5);
      REQUIRE(as_vector<int>(array) == std::vector<int>{6, 7, 8});

      CHECK(array.remove(0) == 6);
      REQUIRE(as_vector<int>(array) == std::vector<int>{7, 8});

      CHECK(array.remove(0) == 7);
      REQUIRE(as_vector<int>(array) == std::vector<int>{8});

      CHECK(array.remove(0) == 8);
      REQUIRE(array.size() == 0);
   }
}

// ------------------------------------------------------------------
TEST_CASE("multiple_insertions", "[matrix_array]") {
   constexpr auto FACTOR{16};
   matrix_array<int, FACTOR> array;
   REQUIRE(array.size() == 0);
   REQUIRE(array.capacity() == FACTOR);

   constexpr auto elems_count{1'000};
   for (size_t i = elems_count; i--;) {
      array.add_back(42);
   }

   REQUIRE(as_vector<int>(array) == std::vector<int>(elems_count, 42));
   REQUIRE(array.size() == elems_count);

   for (size_t i = elems_count; i--;) {
      array.remove(array.size()-1);
   }

   REQUIRE(array.size() == 0);
}



// ------------------------------------------------------------------
TEST_CASE("auxiliary_functions_test", "[heap]") {
   CHECK(parent(1) == 0);
   CHECK(parent(2) == 0);

   CHECK(parent(3) == 1);
   CHECK(parent(4) == 1);

   CHECK(parent(5) == 2);
   CHECK(parent(6) == 2);

   CHECK(left (0) == 1);
   CHECK(right(0) == 2);

   CHECK(left (1) == 3);
   CHECK(right(1) == 4);

   CHECK(left (2) == 5);
   CHECK(right(2) == 6);

   SECTION("is_it_a_heap") {
      const std::vector<int> heap0{100};
      CHECK(is_heap(heap0));

      const std::vector<int> heap1{100, 90};
      CHECK(is_heap(heap1));

      const std::vector<int> heap2{100, 90, 80};
      CHECK(is_heap(heap2));

      const std::vector<int> heap3{100, 90, 80, 85, 84, 77, 76, 84, 83, 80, 79, 75, 74, 70, 69};
      CHECK(is_heap(heap3));

      const std::vector<int> no_heap0{100, 110};
      CHECK(!is_heap(no_heap0));

      const std::vector<int> no_heap1{100, 90, 110};
      CHECK(!is_heap(no_heap1));

      const std::vector<int> no_heap2{100, 90, 80, 85, 84, 77, 76, 84, 83, 80, 79, 110, 74, 70, 69};
      CHECK(!is_heap(no_heap2));
   }

   using storage = vector_array<int, 32>;
   heap<storage> h;

//   const auto print = [](const auto& v){
//      for(const auto& i: v) {
//         std::cerr << i << ' ';
//      }
//      std::cerr << '\n';
//   };

   SECTION("build_heap") {
      h.push(10);
      h.push(20);
      h.push(30);
      h.push(40);
      h.push(50);
      h.push(60);
      h.push(70);
      CHECK(is_heap(h.storage()));

      for (int i = 0; i > 100; ++i) {
         h.push(i);
      }

      CHECK(is_heap(h.storage()));
   }
   SECTION("extract_from_heap") {
      std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
      for(const auto& i: v) {
         h.push(i);
      }
      REQUIRE(is_heap(h.storage()));

      std::reverse(v.begin(), v.end());
      for(const auto& i: v) {
         CHECK(h.top() == i);
         h.pop();
         REQUIRE(is_heap(h.storage()));
      }
   }
   SECTION("heap_size") {
      std::vector<int> v{1, 2, 3, 1, 2, 3};

      for(const auto& i: v) {
         h.push(i);
      }
      CHECK(h.size() == v.size());

      for(size_t i = 0, sz = v.size(); i < sz; ++i){
         h.pop();
      }
      CHECK(h.empty());
   }
}