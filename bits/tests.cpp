#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <common.hpp>
#include <fen.hpp>
#include <king.hpp>
#include <knight.hpp>
#include <long_range_pieces.hpp>

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
      const auto moves_mask = get_king_moves_mask(position.idx);
      const auto moves_count = get_moves_count(moves_mask);
      REQUIRE(moves_mask  == position.moves_mask);
      REQUIRE(moves_count == position.moves_count);
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
      const auto moves_mask  = get_knight_moves_mask(position.idx);
      const auto moves_count = get_moves_count(moves_mask);
      REQUIRE(moves_mask  == position.moves_mask);
      REQUIRE(moves_count == position.moves_count);
   }
}

// ------------------------------------------------------------------
TEST_CASE("fen", "[bit_board]") {
   fen_position positions[] = {
      {"7k/8/8/8/8/8/8/K7", {0,0,0,0,0,1,0,0,0,0,0,9223372036854775808UL}},
      {"8/1k6/ppp4p/8/8/8/1K6/QQQQQQQQ", {0,0,0,0,255,512,148434069749760,0,0,0,0,562949953421312}},
      {"7k/7q/P6r/N6b/B6n/R6p/Q7/K7", {1099511627776,4294967296,16777216,65536,256,1,8388608,2147483648,549755813888,140737488355328,36028797018963968,9223372036854775808UL}},
      {"r1bqkb1r/pppp1ppp/2n2n2/4p3/4P3/2N2N2/PPPP1PPP/R1BQKB1R", {268496640,2359296,36,129,8,16,67272588153323520,39582418599936,2594073385365405696,9295429630892703744UL,576460752303423488,1152921504606846976}},
      {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR", {65280,66,36,129,8,16,71776119061217280,4755801206503243776,2594073385365405696,9295429630892703744UL,576460752303423488,1152921504606846976}},
      {"rnb1kbnr/pppp1ppp/4p3/8/6Pq/5P2/PPPPP2P/RNBQKBNR", {1075879680,66,36,129,8,16,67290111619891200,4755801206503243776,2594073385365405696,9295429630892703744UL,2147483648,1152921504606846976}},
      {"8/3k4/8/8/8/8/1K1B1N2/8", {0,8192,2048,0,0,512,0,0,0,0,0,2251799813685248}},
      {"8/6k1/2PPP3/2PKP3/2PPPn2/8/8/8",{30872694685696,0,0,0,0,34359738368,0,536870912,0,0,0,18014398509481984}},
      {"2b1k3/7p/2p3pP/1pPp1pP1/pP1PpP2/P3P3/8/2B1K3", {141030251888640,0,4,0,0,16,36103744483491840,0,288230376151711744,0,0,1152921504606846976}}
   };

   for (const auto& position: positions) {
      const auto masks = parse_fen(position.fen_str);
      REQUIRE(masks == position.pieces_masks);
   }
}

// ------------------------------------------------------------------
TEST_CASE("long_range_pieces", "[bit_board]") {
   SECTION("allies/enemies detect") {
      CHECK(get_allies ('r') == black_pieces);
      CHECK(get_enemies('r') == white_pieces);

      CHECK(get_allies('K') == white_pieces);
      CHECK(get_enemies('K') == black_pieces);
   }

   SECTION("get pice idx") {
      CHECK(get_piece_idx('K', "8/8/8/8/8/8/8/8"  ) == -1);
      CHECK(get_piece_idx('K', "8/8/8/8/8/8/5K2/8") == 13);
      CHECK(get_piece_idx('K', "8/8/8/8/8/8/K6k/8") ==  8);
      CHECK(get_piece_idx('K', "8/8/8/8/8/8/k6K/8") == 15);
   }

   SECTION("rook moves") {
      CHECK(get_rook_moves_mask(21, "8/5P2/8/8/8/2p2R2/8/8") == 35322362339360UL);
   }

   SECTION("bishop moves") {
      CHECK(get_bishop_moves_mask(20, "8/8/8/8/8/8/8/8") == 424704217196612UL);
   }

   SECTION("queen moves") {
      CHECK(get_queen_moves_mask(20, "8/8/8/8/8/8/8/8") == 1157867469641037908UL);
   }

   SECTION("long range pieces moves") {
      long_range::position positions[] = {
         {"5k2/8/4Q3/8/5B2/2R5/8/3K4", 289360691368494084UL, 145390965166737412UL, 6068863523097809168UL},
         {"5k2/1Pr5/4Q1n1/8/5B2/1R3p2/7q/1bK5", 2207650742786UL, 1275777090881536UL, 6068722785609322512UL},
         {"4k2r/3q4/8/6b1/8/P7/2P2K2/2BQ1n1R", 9259542123273814112UL, 275415828992UL,2261181232846896UL},
         {"8/8/3QRB2/1K6/8/8/1p2P1k1/8", 1157425173223178240UL, 9822351133174399488UL, 3034308066490812424UL},
         {"1kq5/8/8/8/8/3P4/2PRPK2/2NQB3", 0UL, 0UL, 0UL},
         {"2bQn3/2pRp3/3B4/2p1p2k/8/8/8/4K3", 5629499534213120UL, 5629585433559040UL, 1446781380292771840UL},
         {"1k6/8/8/8/8/PPPP4/QRBP4/N1KN4", 2UL, 2UL, 2UL},
         {"K7/1Q6/2B5/3R4/4b3/5q2/6r1/7k", 578722409201797128UL, 1155173313027244032UL, 503562040493867522UL},
         {"8/p5p1/3K1B2/8/1p1Q1R1q/8/1P1P1P1P/2k5", 140930711552UL, 598979096185143296UL, 283794629984256UL},
         {"B6Q/8/4q3/8/2n5/5k2/8/K6R", 36170086419038334UL, 567382630203392UL, 9133476586933486080ULL}
      };

      for (const auto& position: positions) {
         const auto rook_idx = get_piece_idx('R', position.fen);
         CHECK_FALSE(rook_idx == -1);
         CHECK(get_rook_moves_mask(rook_idx, position.fen) == position.rook_moves_mask);

         const auto bishop_idx = get_piece_idx('B', position.fen);
         CHECK_FALSE(bishop_idx == -1);
         CHECK(get_bishop_moves_mask(bishop_idx, position.fen) == position.bishop_moves_mask);

         const auto queen_idx = get_piece_idx('Q', position.fen);
         CHECK_FALSE(queen_idx == -1);
         CHECK(get_queen_moves_mask(queen_idx, position.fen) == position.queen_moves_mask);
      }
   }
}