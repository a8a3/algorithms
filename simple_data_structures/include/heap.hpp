#pragma once

#include <vector>

//       9
//    5     8
//   4 3   6 2
//
//          0 1 2 3 4 5 6
//  heap -> 9 5 8 4 3 6 2

// ------------------------------------------------------------------
constexpr size_t parent(size_t child) {
   return (child - 1) / 2;
}

// ------------------------------------------------------------------
constexpr size_t left(size_t parent) {
   return parent * 2 + 1;
}

// ------------------------------------------------------------------
constexpr size_t right(size_t parent) {
   return left(parent) + 1;
}

namespace {

// TODO check implementation on algoexpert!!
// ------------------------------------------------------------------
bool is_heap(const std::vector<int>& storage, size_t p, size_t sz) {
   const auto l = left(p);
   if (l >= sz) {
      return true;
   }
   if (storage[l] > storage[p]) {
      return false;
   }

   const auto r = right(p);
   if (r >= sz) {
      return true;
   }
   if (storage[r] > storage[p]) {
      return false;
   }
   return is_heap(storage, l, sz) && is_heap(storage, r, sz);
}

} // namespace

// ------------------------------------------------------------------
bool is_heap(const std::vector<int>& storage) {
   return is_heap(storage, 0, storage.size());
}

// ------------------------------------------------------------------
class heap {
public:
   heap() {}

   void push(int val) {
      storage_.push_back(val);
      const auto idx = storage_.size()-1;
      sift_up(idx);
   }

   void pop() {
      if (storage_.size() < 2) {
         return;
      }
      std::swap(storage_.front(), storage_.back());
      storage_.erase(std::prev(storage_.end()));

      sift_down(0);
   }

   int top() const {
      return storage_.empty() ? 0 : storage_.front();
   }

   const std::vector<int>& storage() const {
      return storage_;
   }

private:
   std::vector<int> storage_;

   void sift_up(size_t idx) {
      if (idx == 0) {
         return;
      }
      auto p = parent(idx);

      while(idx > 0 && storage_[idx] > storage_[p]) {
         std::swap(storage_[idx], storage_[p]);
         idx = p;
         p = parent(idx);
      }
   }

   void sift_down(size_t idx) {
      auto swap_with = idx;

      while(true) {
         auto l = left(idx);
         if (l < storage_.size() && storage_[l] > storage_[idx]) {
            swap_with = l;
         }

         auto r = right(idx);
         if (r < storage_.size() && storage_[swap_with] < storage_[r]) {
            swap_with = r;
         }

         if (swap_with == idx) break; // while

         std::swap(storage_[idx], storage_[swap_with]);
         idx = swap_with;
      }
   }
};