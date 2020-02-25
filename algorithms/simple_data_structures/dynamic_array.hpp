#pragma once

#include <functional>
#include <map>

namespace {

template <typename T>
void copy_range(const T* src, T* dst, size_t size) {
   if (src == dst) {
      return;
   }

   for (size_t i = 0; i < size; ++i) {
      *(dst + i) = *(src + i);
   }
}

template <typename T>
void move_right(T* from, size_t count) {
   for (; count > 0; --count) {
      auto& next = *(from + count);
      const auto& prev = *(from + (count-1));
      next = prev;
   }
}

template <typename T>
void move_left(T* from, size_t count) {
   for (size_t i = 0; i < count; ++i) {
      auto& prev = *(from+i);
      const auto& next = *(from+(i+1));
      prev = next;
   }
}

} // anonymous namespace

// ------------------------------------------------------------------
template<typename T>
class array {

public:
   array(size_t sz = 0) : size_(sz) {}

   virtual size_t size    () const                    = 0;
   virtual void   add_back(const T& item)             = 0;
   virtual void   add     (const T& item, size_t idx) = 0;
   virtual T      remove  (size_t idx)                = 0;

   virtual ~array() {delete []arr_;}

   virtual const T& get(size_t idx) const = 0 ;


   const T& operator[](size_t idx) const { return arr_[idx]; }

protected:
    T* arr_{ nullptr };
    size_t size_;
};

// ------------------------------------------------------------------
template<typename T>
class single_array : public array<T> {
public:

   size_t size() const override {
      return array<T>::size_;
   }

   void add_back(const T& item) override {
      add(item, array<T>::size_);
   }

   void add(const T& item, size_t idx) override {
      auto& sz = array<T>::size_;
      if (idx > sz) {
         throw std::out_of_range("incorrect index requested");
      }
      auto& arr = array<T>::arr_;

      T* buf = new T[sz + 1];
      buf[idx] = item;

      copy_range(arr, buf, idx);
      copy_range(arr+idx, buf+(idx+1), sz-idx);

      delete []arr;
      arr = buf;
      ++sz;
    }

    T remove(size_t idx) override {
       auto& sz = array<T>::size_;

       if (idx >= sz) {
          throw std::out_of_range("incorrect index requested");
       }

       auto& arr = array<T>::arr_;

       T result{arr[idx]};
       T* buf = new T[sz - 1];

       copy_range(arr, buf, idx);
       copy_range(arr+(idx+1), buf+idx, sz-(idx+1));

       delete []arr;
       arr = buf;
       --sz;
       return result;
    }

    const T& get(size_t idx) const override {
       if (idx >= array<T>::size_) {
          throw std::out_of_range("incorrect index requested");
       }
       return array<T>::arr_[idx];
    }
};

// ------------------------------------------------------------------
template <typename T, size_t E = 8, typename AllocPolicy = std::plus<size_t>>
class vector_array : public array<T> {

public:
   vector_array() : array<T>(E) {
      array<T>::arr_ = new T[array<T>::size_];
   }

   size_t size() const override {
      return current_size_;
   }

   size_t capacity() const {
      return array<T>::size_;
   }

   void add_back(const T& item) override {
      add(item, current_size_);
   }

   void add(const T& item, size_t idx) override {
      if (idx > current_size_) {
         throw std::out_of_range("incorrect index requested");
      }
      auto& capacity = array<T>::size_;
      auto buf = array<T>::arr_;

      if (current_size_ == capacity) {
         capacity = AllocPolicy()(capacity, E);
         buf = new T[capacity];
      }
      const auto reallocation_happened = buf != array<T>::arr_;

      if (reallocation_happened) {
         copy_range(array<T>::arr_, buf, idx);
         copy_range(array<T>::arr_ + idx, buf + (idx+1), current_size_-idx);
      } else {
         move_right(buf+idx, current_size_ - idx);
      }
      buf[idx] = item;

      if (reallocation_happened) {
         delete []array<T>::arr_;
         array<T>::arr_ = buf;
      }
      ++current_size_;
   }

   T remove(size_t idx) override {
      if (idx >= current_size_) {
         throw std::out_of_range("incorrect index requested");
      }

      auto& arr = array<T>::arr_;
      T result{arr[idx]};

      move_left(arr + idx, current_size_-(idx+1));

      --current_size_;
      return result;
   }

   const T& get(size_t idx) const override {
      if (idx >= current_size_) {
         throw std::out_of_range("incorrect index requested");
      }
      return array<T>::arr_[idx];
   }

private:
   size_t current_size_{0};
};

// ------------------------------------------------------------------
template<typename T, size_t E>
using factor_array = vector_array<T, E, std::multiplies<size_t>>;

// TODO
//class matrix_array {};