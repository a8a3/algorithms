#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>
#include <shuffle.hpp>

#include <vector>

#include <insertion_sort.hpp>
#include <shell_sort.hpp>

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
      make_shuffle(v);
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

   SECTION("shuffled array") {
      make_shuffle(a);
      shell_sort<TestType>(a, sizeof(a) / sizeof(a[0]));
      CHECK(equal(a, e));
   }

   SECTION("ascending sorted array") {
      shell_sort<TestType>(a, sizeof(a) / sizeof(a[0]));
      CHECK(equal(a, e));
   }

   SECTION("descending sorted array") {
      std::reverse(std::begin(a), std::end(a));
      shell_sort<TestType>(a, sizeof(a) / sizeof(a[0]));
      CHECK(equal(a, e));
   }
}