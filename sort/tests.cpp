#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>
#include <heap_sort.hpp>
#include <insertion_sort.hpp>
#include <shell_sort.hpp>
#include <shuffle.hpp>
#include <vector>

// ------------------------------------------------------------------
TEST_CASE("insertion", "[sort]") {
   SECTION("empty array") {
      std::vector<int> e;
      std::vector<int> v = e;
      insertion_sort(v);
      CHECK(v == e);
   }
   std::vector<int> e{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
   SECTION("shuffled array") {
      std::vector<int> v{e.begin(), e.end()};
      make_shuffle(v.data(), v.size());
      insertion_sort(v);
      CHECK(v == e);
   }
   SECTION("sorted array") {
      std::vector<int> v{e.begin(), e.end()};
      insertion_sort(v);
      CHECK(v == e);
   }
}

// ------------------------------------------------------------------
TEST_CASE("gaps", "[sort]") {
   SECTION("shell gaps") {
      gap::shell sh(100);
      for (size_t i = 10; i; --i) {
         std::clog << sh.next_gap() << ' ';
      }
      std::clog << '\n';
   }
   SECTION("hibbard gaps") {
      gap::hibbard h(100);
      for (size_t i = 10; i; --i) {
         std::clog << h.next_gap() << ' ';
      }
      std::clog << '\n';
   }
   SECTION("sadgewick gaps") {
      gap::sedgewick sd(100);
      for (size_t i = 10; i; --i) {
         std::clog << sd.next_gap() << ' ';
      }
      std::clog << '\n';
   }
}

// -----------------------------------
TEMPLATE_TEST_CASE("shell", "[sort][template]", gap::shell, gap::hibbard, gap::sedgewick) {
   SECTION("empty array") {
      int a[] = {0};
      shell_sort<TestType>(a, 0);
   }

   int e[] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 10, 11, 12, 13, 14, 15};
   int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 10, 11, 12, 13, 14, 15};

   const auto sz = sizeof(a) / sizeof(a[0]);

   SECTION("shuffled array") {
      make_shuffle(a, sz);
      shell_sort<TestType>(a, sz);
      CHECK(equal(a, e));
   }

   SECTION("ascending sorted array") {
      shell_sort<TestType>(a, sz);
      CHECK(equal(a, e));
   }

   SECTION("descending sorted array") {
      std::reverse(std::begin(a), std::end(a));
      shell_sort<TestType>(a, sz);
      CHECK(equal(a, e));
   }
}

// ------------------------------------------------------------------
TEST_CASE("naive_heap_sort", "[sort]") {

   int origin[] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 10, 11, 12, 13, 14, 15};
   int mixed [] = {0, 1, 2, 3, 4, 5, 6, 7, 7, 7, 10, 11, 12, 13, 14, 15};
   const size_t sz = sizeof(mixed) / sizeof(mixed[0]);

   SECTION("shuffled array") {
      make_shuffle(mixed, sz);
      naive_heap_sort(mixed, sz);
      CHECK(equal(origin, mixed));
   }

   SECTION("ascending sorted array") {
      naive_heap_sort(mixed, sz);
      CHECK(equal(origin, mixed));
   }

   SECTION("descending sorted array") {
      std::reverse(std::begin(mixed), std::end(mixed));
      naive_heap_sort(mixed, sz);
      CHECK(equal(origin, mixed));
   }
}

// ------------------------------------------------------------------
TEST_CASE("heap_sort", "[sort]") {
   constexpr auto sz = 10'000;
   std::unique_ptr<int[]> arr(new int[sz]);
   std::iota(arr.get(), arr.get() + sz, 0);

   CHECK(std::is_sorted(arr.get(), arr.get()+sz));

   SECTION("shuffled array") {
      make_shuffle(arr.get(), sz);
      CHECK_FALSE(std::is_sorted(arr.get(), arr.get()+sz));
      heap_sort::sort(arr.get(), sz);
      CHECK(std::is_sorted(arr.get(), arr.get()+sz));
   }

   SECTION("ascending sorted array") {
      heap_sort::sort(arr.get(), sz);
      CHECK(std::is_sorted(arr.get(), arr.get()+sz));
   }

   SECTION("descending sorted array") {
      std::reverse(arr.get(), arr.get()+sz);
      CHECK_FALSE(std::is_sorted(arr.get(), arr.get()+sz));

      heap_sort::sort(arr.get(), sz);
      CHECK(std::is_sorted(arr.get(), arr.get()+sz));
   }
}

