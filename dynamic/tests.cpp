#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <digits_count.hpp>
#include <islands.hpp>
#include <free_area.hpp>
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

// ------------------------------------------------------------------
TEST_CASE("free_area", "[dynamic]") {
   free_area::area a{{
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 0, 0, 0}
   }};
   CHECK(free_area::calc_area(a) ==
   free_area::area{
      {4, 3, 2, 1},
      {1, 0, 2, 1},
      {0, 3, 2, 1}}
   );
   CHECK(free_area::max_square(a) == 6);

   free_area::area a1{{
      {0, 0, 1, 0},
      {0, 1, 0, 0},
      {1, 0, 0, 0}
   }};
   CHECK(free_area::calc_area(a1) ==
   free_area::area{
      {2, 1, 0, 1},
      {1, 0, 2, 1},
      {0, 3, 2, 1}}
   );
   CHECK(free_area::max_square(a1) == 4);
}

// ------------------------------------------------------------------
TEST_CASE("column_squares", "[dynamic]") {
   free_area::points p{{
      {1, 1},
      {2, 0}
   }};
   CHECK(free_area::column_squares(3, 4, p) ==
      free_area::area{
         {1, 1, 1, 1},
         {2, 0, 2, 2},
         {0, 1, 3, 3}}
   );
}

// ------------------------------------------------------------------
TEST_CASE("left-right width", "[dynamic]") {
   free_area::row r{1, 1, 1, 4, 4, 2, 3, 3, 2, 2};
   free_area::row left;
   free_area::row right;

   std::tie(left, right) = free_area::left_right_boards(r);

   CHECK(left  == free_area::row{0, 0, 0, 3, 3, 3, 6, 6, 3, 3});
   CHECK(right == free_area::row{9, 9, 9, 4, 4, 9, 7, 7, 9, 9});
}

// ------------------------------------------------------------------
TEST_CASE("find max free area", "[dynamic]") {
   free_area::points p{{
      {1, 1},
      {2, 0}
   }};
   const auto max_area = free_area::max_square_ex(3, 4, p);
   CHECK(max_area == 6);
}