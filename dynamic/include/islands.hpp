#pragma once

#include <array>

template<size_t M, size_t N>
using area = std::array<std::array<int, N>, M>;

// ------------------------------------------------------------------
template <size_t M, size_t N>
void go(area<M, N>& a, size_t x, size_t y) {
   if (x < 0 || x >= M) return;
   if (y < 0 || y >= N) return;
   if (0 == a[x][y]) return;

   a[x][y] = 0;
   go(a, x+1, y);
   go(a, x-1, y);
   go(a, x,   y+1);
   go(a, x,   y-1);
}

// ------------------------------------------------------------------
template <size_t M, size_t N>
size_t islands_count(area<M, N> a) {
   size_t islands = 0;

   for(size_t i = 0; i < M; ++i) {
      for(size_t j = 0; j < N; ++j) {
         if (a[i][j]) {
            // island detected
            ++islands;
            go<M, N>(a, i, j);
         }
      }
   }
   return islands;
}
