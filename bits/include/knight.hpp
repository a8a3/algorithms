#pragma once

// ------------------------------------------------------------------
uint64_t get_knight_turns_mask(uint8_t idx) {
#if 0

8 * * * * * * * *
7 * * h * a * * *
6 * g * * * b * *
5 * * * K * * * *
4 * f * * * c * *
3 * * e * d * * *
2 * * * * * * * *
1 * * * * * * * *
  A B C D E F G H

#endif
   const uint64_t nA = 0xFEFE'FEFE'FEFE'FEFEUL;
   const uint64_t nB = 0xFDFD'FDFD'FDFD'FDFDUL;
   const uint64_t nG = 0xBFBF'BFBF'BFBF'BFBFUL;
   const uint64_t nH = 0x7F7F'7F7F'7F7F'7F7FUL;

   const uint64_t  K = 1UL << idx;

   const uint64_t  a = (K & nH)      << 17;
   const uint64_t  b = (K & nG & nH) << 10;

   const uint64_t  c = (K & nG & nH) >>  6;
   const uint64_t  d = (K & nH)      >> 15;

   const uint64_t  e = (K & nA)      >> 17;
   const uint64_t  f = (K & nA & nB) >> 10;

   const uint64_t  g = (K & nA & nB) <<  6;
   const uint64_t  h = (K & nA)      << 15;

   return a | b | c |
          d | e | f |
          g | h ;
}