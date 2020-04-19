#pragma once

#include <swap.hpp>

namespace {

template<typename T>
int separate(T arr[], int l, int r) {
   int i = l - 1;
   int j = i + 1;
   const int pivot = arr[r];

   while (j <= r) {
      if (arr[j] <= pivot) {
         ++i;
         utils::swap(arr + i, arr + j);
      }
      ++j;
   }
   return i;
}

template<typename T>
void sort_helper(T arr[], int l, int r) {
   if (l >= r) {
      return;
   }
   const int idx = separate(arr, l, r);
   sort_helper(arr, l,       idx - 1);
   sort_helper(arr, idx + 1, r      );
}

} // namespace


// ------------------------------------------------------------------
class quick_sort {
public:

template<typename T>
static void sort(T arr[], int size) {
   sort_helper(arr, 0, size-1);
}
};