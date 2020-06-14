#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <digits_count.hpp>
#include <islands.hpp>
#include <tree.hpp>

// ------------------------------------------------------------------
TEST_CASE("tree", "[dynamic]") {

   constexpr auto N = 5;
   int tree[N][N] = {
      {  1, 0, 0, 0, 0},
      {  1, 2, 0, 0, 0},
      {  1, 2, 3, 0, 0},
      {  1, 2, 3, 4, 0},
      {100, 2, 3, 4, 5}
   };
   CHECK(max_path(tree) == 104);
}

// ------------------------------------------------------------------
TEST_CASE("digits_count", "[dynamic]") {
   std::vector<std::pair<int, long>> data {
      {1, 2},
      {2, 4},
      {3, 6},
      {5, 16},
      {10,178},
      {88, 3'559'958'832'009'428'378}};

   std::for_each(data.begin(), data.end(),
   [](const auto& i){
      CHECK(get_digits_count(i.first) == i.second);
   });
}

// ------------------------------------------------------------------
TEST_CASE("islands", "[dynamic]") {
   area<4, 4> a{{
      {1, 1, 1, 1},
      {1, 1, 1, 1},
      {1, 1, 1, 1},
      {1, 1, 1, 1}
   }};
   CHECK(islands_count(a) == 1);

   area<4, 4> a1{{
      {1, 0, 1, 0},
      {0, 1, 0, 1},
      {1, 0, 1, 0},
      {0, 1, 0, 1}
   }};
   CHECK(islands_count(a1) == 8);

   area<4, 4> a2{{
      {1, 0, 0, 1},
      {1, 0, 0, 1},
      {1, 0, 0, 1},
      {1, 1, 1, 1}
   }};
   CHECK(islands_count(a2) == 1);
}

