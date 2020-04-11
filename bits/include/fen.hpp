#pragma once

#include <algorithm>
#include <array>
#include <sstream>
#include <string>

// ------------------------------------------------------------------
struct fen_position {
   constexpr static auto rows_count   = 8;
   constexpr static auto pieces_count = 12;
   constexpr static auto delimeter    = '/';

   const std::string fen_str;
   const std::array<uint64_t, pieces_count> pieces_masks;
};

using fp = fen_position;

std::array<char, fp::pieces_count> all_pieces{'P', 'N', 'B', 'R', 'Q', 'K',
                                              'p', 'n', 'b', 'r', 'q', 'k'};

constexpr auto white_pieces = "PNBRQK";
constexpr auto black_pieces = "pnbrqk";

// ------------------------------------------------------------------
uint64_t get_pieces_mask(const std::string& what_pieces, const std::string &fen_str) {

   std::istringstream ss(fen_str);
   std::string row;

   uint64_t res = 0;

   for (int row_idx = fp::rows_count-1; row_idx >= 0; --row_idx) {
      std::getline(ss, row, fp::delimeter);

      size_t piece_pos = 0;
      for (size_t j = 0, sz = row.size(); j < sz; ++j) {
         const char symb = row[j];
         if (std::any_of(all_pieces.begin(), all_pieces.end(), [&symb](const auto piece) {return piece == symb; })) {
            if (what_pieces.find(symb) != std::string::npos) {
               res |= 1UL << (row_idx*fp::rows_count + piece_pos);
            }
            piece_pos += 1;
         } else {
            piece_pos += std::stoi(std::string().append(1, symb));
         }
      }
   }
   return res;
}

// ------------------------------------------------------------------
std::array<uint64_t, fp::pieces_count> parse_fen(const std::string& str) {
   std::array<uint64_t, fp::pieces_count> res;

   for (size_t i = 0; i < fp::pieces_count; ++i) {
      res[i] = get_pieces_mask(std::string().append(1, all_pieces[i]), str);
   }
   return res;
}

// ------------------------------------------------------------------
int get_piece_idx(char piece, const std::string& fen_str) {
   std::istringstream ss(fen_str);
   std::string row;

   for (int row_idx = fp::rows_count-1; row_idx >= 0; --row_idx) {
      std::getline(ss, row, fp::delimeter);
      int piece_row_pos = 0;

      for (size_t j = 0, sz = row.size(); j < sz; ++j) {
         const char symb = row[j];
         if (std::any_of(all_pieces.begin(), all_pieces.end(), [&symb](const auto piece) {return piece == symb;})) {
            if (symb == piece) {
               return fp::rows_count*row_idx + piece_row_pos;
            }
            piece_row_pos += 1;
         } else {
            piece_row_pos += std::stoi(std::string().append(1, symb));
         }
      }
   }
   return -1;
}
