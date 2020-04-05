#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <common.hpp>
#include <king.hpp>
#include <knight.hpp>

// ------------------------------------------------------------------
TEST_CASE("king", "[bit_board]") {

   position positions[] = {
      { 0, 3, 770UL},
      { 1, 5, 1797UL},
      { 7, 3, 49216UL},
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
      const auto turns_count = get_turns_count(turns_mask);
      REQUIRE(turns_mask  == position.turns_mask);
      REQUIRE(turns_count == position.turns_count);
   }
}

// ------------------------------------------------------------------
TEST_CASE("knight", "[bit_board]") {
   position positions[] = {
      { 0, 2,132096UL},
      { 1, 3,329728UL},
      { 2, 4,659712UL},
      {36, 8,11333767002587136UL},
      {47, 4,4620693356194824192UL},
      {48, 3,288234782788157440UL},
      {54, 4,1152939783987658752UL},
      {55, 3,2305878468463689728UL},
      {56, 2,1128098930098176UL},
      {63, 2,9077567998918656UL}
   };

   for (const auto& position: positions) {
      const auto turns_mask  = get_knight_turns_mask(position.idx);
      const auto turns_count = get_turns_count(turns_mask);
      REQUIRE(turns_mask  == position.turns_mask);
      REQUIRE(turns_count == position.turns_count);
   }
}