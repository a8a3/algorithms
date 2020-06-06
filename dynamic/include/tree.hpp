#pragma once

#include <algorithm>

// ------------------------------------------------------------------
template<size_t N>
int max_path(int (&arr)[N][N]) {
   static_assert(N > 0, "N must be greater than zero");

   for (int i = N-2; i >= 0; --i) {
      for(int j = 0; j <= i; ++j) {
         arr[i][j] += std::max(arr[i+1][j], arr[i+1][j+1]);
      }
   }
   return arr[0][0];
}