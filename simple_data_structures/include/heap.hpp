#pragma once

#include "dynamic_array.hpp"

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
template<typename T>
bool is_heap(const T& storage, size_t p, size_t sz) {
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
template<typename T>
bool is_heap(const T& storage) {
   return is_heap(storage, 0, storage.size());
}

// ------------------------------------------------------------------
template<typename T, typename Tstorage = vector_array<T, 32>>
class heap {
public:
   void push(const T& val) {
      storage_.add_back(val);
      const auto idx = storage_.size()-1;
      sift_up(idx);
   }

   void pop() {
      if (::empty(storage_)) {
         return;
      }

      const auto last_idx = storage_.size() - 1;
      std::swap(storage_[0], storage_[last_idx]);
      storage_.remove(last_idx);
      sift_down(0);
   }

   T top() const {
      return storage_.get(0);
   }

   size_t size() const noexcept {
      return storage_.size();
   }

   bool empty() const noexcept {
      return ::empty(storage_);
   }

   const Tstorage& storage() const {
      return storage_;
   }

private:
   Tstorage storage_;

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