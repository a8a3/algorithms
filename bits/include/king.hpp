#pragma once

// ------------------------------------------------------------------
uint64_t get_king_turns_mask(uint8_t idx) {
#if 0

lu u ru
 l K  r
ld d rd

#endif
   const uint64_t nA = 0xFEFEFEFEFEFEFEFEUL;
   const uint64_t nH = 0x7F7F7F7F7F7F7F7FUL;
   const uint64_t  K = 1UL << idx;

   const uint64_t  u = K << 8;
   const uint64_t lu = (K & nA) ? (u >> 1) : 0;
   const uint64_t ru = (K & nH) ? (u << 1) : 0;

   const uint64_t  r = (K & nH) ? (K << 1) : 0;
   const uint64_t  l = (K & nA) ? (K >> 1) : 0;

   const uint64_t  d = K >> 8;
   const uint64_t ld = (K & nA) ? (d >> 1) : 0;
   const uint64_t rd = (K & nH) ? (d << 1) : 0;

   return lu | u | ru |
           l |      r |
          ld | d | rd;
}
