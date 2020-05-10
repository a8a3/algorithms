#pragma once

#include <algorithm>
#include <memory>

namespace segment {

#if 0
          36
      10       26
    3   7   11  15
   1 2 3 4 5 6 7 8
#endif // 0

// ------------------------------------------------------------------
uint32_t get_parent(uint32_t child) noexcept {
   return (child)/2;
}

// ------------------------------------------------------------------
uint32_t get_left(uint32_t parent) noexcept {
   return parent * 2;
}

// ------------------------------------------------------------------
uint32_t get_right(uint32_t parent) noexcept {
  return get_left(parent) + 1;
}

// ------------------------------------------------------------------
bool is_right(uint32_t idx) noexcept {
   return (idx % 2) > 0;
}

// ------------------------------------------------------------------
bool is_left(uint32_t idx) noexcept {
   return !is_right(idx);
}

// ------------------------------------------------------------------
uint64_t next_pow_of2(uint64_t val) noexcept {
   if (val && !(val & (val-1))) {
      return val;
   }

   uint64_t next = 1;
   while(next < val) {
      next <<= 1;
   }
   return next;
}

// ------------------------------------------------------------------
class tree {

public:
   explicit tree(size_t sz) : sz_(sz) {
      const auto storage_sz = next_pow_of2(sz) * 2;
      storage_.reset(new uint64_t[storage_sz]);
      uint64_t* p_storage = storage_.get();
      std::fill(p_storage, p_storage + storage_sz, 0);
   }

   tree(const int arr[], size_t sz) : sz_(sz) {
      const auto storage_sz = next_pow_of2(sz) * 2;
      storage_.reset(new uint64_t[storage_sz]);
      uint64_t* p_storage = storage_.get();

      std::fill(p_storage, p_storage + storage_sz, 0);
      std::copy_n(arr, sz, p_storage + sz);

      // calc sums
      for (size_t i = sz-1; i > 0; --i) {
         p_storage[i] = p_storage[get_left(i)] + p_storage[get_right(i)];
      }
   }

   uint64_t calc(uint32_t l, uint32_t r) const noexcept {
      uint64_t sum = 0;

      l += sz_;
      r += sz_;

      while(l <= r) {
         if (is_right(l)) {
            sum += storage_[l];
         }

         if (is_left(r)) {
            sum += storage_[r];
         }

         l = get_parent(l + 1);
         r = get_parent(r - 1);
      }
      return sum;
   }

   void set(uint32_t idx, uint64_t val) noexcept {
      idx += sz_;
      storage_[idx] = val;

      idx = get_parent(idx);

      while (idx > 0) {
         storage_[idx] = storage_[get_left(idx)] + storage_[get_right(idx)];
         idx = get_parent(idx);
      }
   }

private:
   std::unique_ptr<uint64_t []> storage_;
   const size_t sz_;
};


} // namespace segment