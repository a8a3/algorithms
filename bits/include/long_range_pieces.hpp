#pragma once

#include "fen.hpp"

namespace long_range {

struct position {
   std::string fen;
   uint64_t rook_moves_mask{0};
   uint64_t bishop_moves_mask{0};
   uint64_t queen_moves_mask{0};
};

} // namespace long_range

// ------------------------------------------------------------------
const char* get_allies(char piece) {
   return std::isupper(piece) ? white_pieces : black_pieces;
}

// ------------------------------------------------------------------
const char* get_enemies(char piece) {
   return std::isupper(piece) ? get_allies(std::tolower(piece))
                              : get_allies(std::toupper(piece));
}

// ------------------------------------------------------------------
uint64_t get_allies_mask(char piece, const std::string& fen_str) {
   return get_pieces_mask(get_allies(piece), fen_str);
}

// ------------------------------------------------------------------
uint64_t get_enemies_mask(char piece, const std::string& fen_str) {
   return get_pieces_mask(get_enemies(piece), fen_str);
}

// ------------------------------------------------------------------
bool apply_move(uint64_t idx, uint64_t& moves_mask, uint64_t allies, uint64_t enemies) {
   const auto move = 1UL << idx;
   if (move & allies) {  // stop before allies
      return false;
   }
   moves_mask |= move;
   return (move & enemies) == 0; // eat enemy and stop, continue otherwise
}


// ------------------------------------------------------------------
uint64_t get_rook_moves_mask(uint8_t idx, const std::string& fen_str) {
#if 0
fen: 8/5P2/8/8/8/2p2R2/8/8

8 * * * * * * * *
7 * * * * * P * *
6 * * * * * + * *
5 * * * * * + * *
4 * * * * * + * *
3 * *p+ + + R + +  idx = 21, rook_row_idx = 2, rook_col_idx = 5
2 * * * * * + * *
1 * * * * * + * *
  A B C D E F G H
#endif

   const auto allies_mask = get_allies_mask('R', fen_str);
   const auto enemies_mask = get_enemies_mask('R', fen_str);

   constexpr int8_t board_side = 8;

   const int8_t rook_row_idx = idx / board_side;   // 0 - 7
   const int8_t rook_col_idx = idx % board_side;   // 0 - 7

   uint64_t res = 0;

   // moves up
   for (int i = rook_row_idx+1; i < board_side; ++i) {
      if (!apply_move(rook_col_idx + board_side*i, res, allies_mask, enemies_mask)) {
         break; // for
      }
   }
   // moves down
   for (int i = rook_row_idx-1; i >= 0; --i) {
      if (!apply_move(rook_col_idx + board_side*i, res, allies_mask, enemies_mask)) {
         break; // for
      }
   }
   // moves left
   for (int i = rook_col_idx-1; i >= 0; --i) {
      if (!apply_move(rook_row_idx*board_side + i, res, allies_mask, enemies_mask)) {
         break; // for
      }
   }
   // moves right
   for (int i = rook_col_idx+1; i < board_side; ++i) {
      if (!apply_move(rook_row_idx*board_side + i, res, allies_mask, enemies_mask)) {
         break; // for
      }
   }
   return res;
}

// ------------------------------------------------------------------
uint64_t get_bishop_moves_mask(uint8_t idx, const std::string& fen_str) {
#if 0

fen: 8/8/8/8/8/5B2/8/8
 u
8  + * * * * * * *
7  * + * * * * * *
6  * * + * * * * * r
5  * * * + * * * +
4  * * * * + * + *
3  * * * * * B * *  idx = 21, row_idx = 2, col_idx = 5
2  * * * * + * + *
1  * * * + * * * +
       l           d (min(row, (board_side - (col + 1))))
  A B C D E F G H

d (min(board_side-(col+1), row))
l (min(col               , row))
u (min(col               , board_side-(row+1)))
r (min(board_side-(col+1), board_side-(row+1)))

#endif
   const auto allies_mask  = get_allies_mask ('B', fen_str);
   const auto enemies_mask = get_enemies_mask('B', fen_str);

   constexpr int board_side = 8;

   const int row_idx = idx / board_side;   // 0 - 7
   const int col_idx = idx % board_side;   // 0 - 7

   uint64_t res = 0;

   // moves up
   for (int i = 1, sz = std::min(col_idx, board_side-(row_idx+1)); i <= sz; ++i) {
      if (!apply_move(idx + (board_side-1)*i, res, allies_mask, enemies_mask)) {
         break; // for
      }
   }

   // moves down
   for (int i = 1, sz = std::min(board_side-(col_idx+1), row_idx); i <= sz; ++i) {
      if (!apply_move(idx - (board_side-1)*i, res, allies_mask, enemies_mask)) {
         break; // for
      }
   }

   // moves left
   for (int i = 1, sz = std::min(col_idx, row_idx); i <= sz; ++i) {
      if (!apply_move(idx - (board_side+1)*i, res, allies_mask, enemies_mask)) {
         break; // for
      }
   }

   // moves right
   for (int i = 1, sz = std::min(board_side-(col_idx+1), board_side-(row_idx+1)); i <= sz; ++i) {
      if (!apply_move(idx + (board_side+1) * i, res, allies_mask, enemies_mask)) {
         break; // for
      }
   }
   return res;
}

// ------------------------------------------------------------------
uint64_t get_queen_moves_mask(uint8_t idx, const std::string& fen_str) {
   return get_bishop_moves_mask(idx, fen_str) | get_rook_moves_mask(idx, fen_str);
}