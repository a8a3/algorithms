#pragma once

// ------------------------------------------------------------------
struct king_position {
   uint8_t  idx;
   uint8_t  turns_count;
   uint64_t turns_mask;
};

// ------------------------------------------------------------------
uint64_t get_king_turns_mask(uint8_t /*idx*/) {
   return 0;
}

// ------------------------------------------------------------------
uint8_t get_king_turns_count(uint64_t /*mask*/) {
   return 0;
}