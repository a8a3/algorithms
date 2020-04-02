#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <king.hpp>

// ------------------------------------------------------------------
TEST_CASE("king", "[bitboard]") {

   king_position positions[] = {
      { 0, 3, 770UL},
      { 1, 5, 1797UL},
      { 2, 3, 49216UL},
      { 8, 5, 197123UL},
      {10, 8, 920078UL},
      {15, 5, 12599488UL},
      {54, 8, 16186183351374184448UL},
      {55, 5, 13853283560024178688UL},
      {56, 3, 144959613005987840UL},
      {63, 3, 4665729213955833856UL}
   };

   for (const auto& position: positions) {
      const auto turns_mask = get_king_turns_mask(position.idx);
      const auto turns_count = get_king_turns_count(turns_mask);
      REQUIRE(turns_mask == position.turns_mask);
      REQUIRE(turns_count == position.turns_count);
   }


}

