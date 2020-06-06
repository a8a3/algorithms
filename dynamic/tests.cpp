#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

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
