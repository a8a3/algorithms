#pragma once

#include <iostream>

// ----------------------------------------------
namespace {
void swap(int *a, int *b) {
   int tmp = *a;
   *a = *b;
   *b = tmp;
}
} // namespace


// ----------------------------------------------
void naive_heap_sort(int* arr, size_t sz) {

   const auto sift_up = [](int* arr, size_t sz) {
      for (size_t i = 1; i < sz; ++i) {
         if (*(arr + i) > *arr) {
            swap(arr+i, arr);
         }
      }
   };

   for(size_t i = 0; i < sz; ++i) {
      sift_up(arr, sz - i);
      swap(arr, arr + (sz-1) - i);
   }
}

// ----------------------------------------------
class heap_sort{
   static void sift_down(size_t idx, int* arr, size_t sz) {
      const auto l = idx*2 + 1;
      const auto r = l + 1;

      auto max = idx;

      if (l < sz && arr[l] > arr[max]) max = l;
      if (r < sz && arr[r] > arr[max]) max = r;

      if (max == idx) return;

      swap(arr + idx, arr + max);
      sift_down(max, arr, sz);
   };

   static void print(int* arr, size_t sz) {
      for(size_t i = 0; i < sz; ++i) {
         std::clog << arr[i] << ' ';
      }
      std::clog << '\n';
   }
public:

   static void sort(int* arr, size_t sz) {
      // build a heap
      for (int i = sz/2 - 1; i >= 0; --i) {
         sift_down(i, arr, sz);
      }

      // pick top and rebuild heap
      for(size_t i = 0; i < sz - 1; ++i) {
         swap(arr, arr + sz-(i+1) );
         sift_down(0, arr, sz-(i+1));
      }
   }
};

