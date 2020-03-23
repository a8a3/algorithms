#pragma once

#include <vector>

// ------------------------------------------------------------------
void swap(int& lhs, int& rhs) {
   auto tmp = lhs;
   lhs = rhs;
   rhs = tmp;
}

// ------------------------------------------------------------------
void insertion_sort(std::vector<int>& v) {
   size_t j, i = 1;

   while (i < v.size()) {
      j = i;

      while (j > 0) {
         if (v[j] < v[j-1]) {
            swap(v[j], v[j-1]);
         }
         --j;
      }
      ++i;
   }
}