#pragma once

#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

template<typename T>
void merge(T a[], size_t a_size, T b[], size_t b_size, T c[]) {
   size_t a_idx{0};
   size_t b_idx{0};
   size_t c_idx{0};

   while(a_idx < a_size && b_idx < b_size) {
      if (a[a_idx] < b[b_idx]) {
         c[c_idx] = a[a_idx];
         ++a_idx;
      } else {
         c[c_idx] = b[b_idx];
         ++b_idx;
      }
      ++c_idx;
   }
   while(a_idx < a_size) {
      c[c_idx] = a[a_idx];
      ++a_idx;
      ++c_idx;
   }
   while(b_idx < b_size) {
      c[c_idx] = b[b_idx];
      ++b_idx;
      ++c_idx;
   }
}

// ------------------------------------------------------------------
class merge_sort {
public:
   static void sort(int a[], size_t sz) {
      if (sz < 2) return;

      const auto center = sz/2;
      sort(a, center);
      sort(a + center, sz - center);

      std::unique_ptr<int[]> tmp(new int[sz]);
      merge(a, center, a+center, sz-center, tmp.get());

      for (size_t i = 0; i < sz; ++i) {
         a[i] = tmp[i];
      }
   }
};
