#pragma once

#include "heap.hpp"

// ------------------------------------------------------------------
template <typename T>
class priority_queue {
   heap<T> storage_;

public:
   void enqueue(const T& obj) {
      storage_.push(obj);
   }
   T dequeue() {
      T result = storage_.top();
      storage_.pop();
      return result;
   }
   size_t size() const noexcept {
      return storage_.size();
   }
   bool empty() const noexcept {
      return ::empty(storage_);
   }
};


// ------------------------------------------------------------------
template <typename T, size_t N>
class stable_priority_queue {
   vector_array<T, N> storage_;

public:
   void enqueue(const T& obj) {
      storage_.add_back(obj);

      auto idx = storage_.size() - 1;
      while(idx > 0 && storage_[idx] > storage_[idx-1]) {
         std::swap(storage_[idx], storage_[idx-1]);
         idx -= 1;
      }
   }
   T dequeue() {
      return storage_.remove(0);
   }
   size_t size() const noexcept {
      return storage_.size();
   }
   bool empty() const noexcept {
      return ::empty(storage_);
   }
};