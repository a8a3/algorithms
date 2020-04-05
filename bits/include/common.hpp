#pragma once

// ------------------------------------------------------------------
struct position {
   uint8_t  idx;
   uint8_t  turns_count;
   uint64_t turns_mask;
};

// ------------------------------------------------------------------
int get_turns_count(uint64_t turns_mask) {
   int res{0};

   while(turns_mask > 0) {
      turns_mask &= turns_mask-1;
      ++res;
   }
   return res;
}