#pragma once

// ------------------------------------------------------------------
struct position {
   uint8_t  idx;
   uint8_t  moves_count;
   uint64_t moves_mask;
};

// ------------------------------------------------------------------
int get_moves_count(uint64_t moves_mask) {
   int res{0};

   while(moves_mask > 0) {
      moves_mask &= moves_mask-1;
      ++res;
   }
   return res;
}