#pragma once

#include <functional>
#include <stdexcept>
#include <vector>

namespace {

template <typename T>
void copy_range(const T* src, T* dst, size_t size) {
   if (src == dst) {
      return;
   }

   for (size_t i = 0; i < size; ++i) {
      dst[i] = src[i];
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

} // namespace


// ------------------------------------------------------------------
template<typename T>
class single_array {
public:

   single_array() = default;

   single_array(const single_array& other) {
      *this = other;
   }

   single_array&operator=(const single_array& other) {
      if (&other == this) {
         return *this;
      }

      delete[] arr_;
      arr_ = new T[other.size_];
      size_ = other.size_;
      copy_range(other.arr_, arr_, other.size_);
      return *this;
   }

   ~single_array() {delete []arr_;}

   size_t size() const noexcept {
      return size_;
   }

   void add_back(const T& item) {
      add(item, size_);
   }

   void add_front(const T& item) {
      add(item, 0);
   }

   void add(const T& item, size_t idx) {
      auto& sz = size_;
      if (idx > sz) {
         throw std::out_of_range("incorrect index requested");
      }
      auto& arr = arr_;

      T* buf = new T[sz + 1];
      buf[idx] = item;

      copy_range(arr, buf, idx);
      copy_range(arr+idx, buf+(idx+1), sz-idx);

      delete []arr;
      arr = buf;
      ++sz;
    }

    T remove(size_t idx) {
       if (idx >= size_) {
          throw std::out_of_range("incorrect index requested");
       }

       T result{arr_[idx]};
       T* buf = new T[size_ - 1];

       copy_range(arr_, buf, idx);
       copy_range(arr_+(idx+1), buf+idx, size_ - (idx+1));

       delete []arr_;
       arr_ = buf;
       --size_;
       return result;
    }

    const T& get(size_t idx) const {
       if (idx >= size_) {
          throw std::out_of_range("incorrect index requested");
       }
       return arr_[idx];
    }

          T& operator[](std::size_t idx)       { return arr_[idx]; }
    const T& operator[](std::size_t idx) const { return arr_[idx]; }

private:
    T*     arr_ {nullptr};
    size_t size_{0};
};

// ------------------------------------------------------------------
template <typename T, size_t E = 8, typename AllocPolicy = std::plus<size_t>>
class vector_array {
public:
   vector_array() : capacity_(E) {
      arr_ = new T[capacity_];
   }

   vector_array(const vector_array& other) {
      *this = other;
   }

   vector_array& operator= (const vector_array& other) {
      if (&other == this) {
         return *this;
      }

      delete []arr_;
      arr_ = new T[other.size_];
      size_ = other.size_;

      copy_range(other.arr_, arr_, other.size_);
      return *this;
   }

   ~vector_array() {delete []arr_;}

   size_t size() const noexcept {
      return size_;
   }

   size_t capacity() const {
      return capacity_;
   }

   void add_back(const T& item) {
      add(item, size_);
   }

   void add_front(const T& item) {
      add(item, 0);
   }

   void add(const T& item, size_t idx) {
      if (idx > size_) {
         throw std::out_of_range("incorrect index requested");
      }
      auto capacity = capacity_;
      auto buf = arr_;

      if (size_ == capacity) {
         capacity = AllocPolicy()(capacity, E);
         buf = new T[capacity];
      }
      const auto reallocation_happened = buf != arr_;

      if (reallocation_happened) {
         copy_range(arr_, buf, idx);
         copy_range(arr_ + idx, buf + (idx+1), size_-idx);
      } else {
         move_right(buf+idx, size_ - idx);
      }
      buf[idx] = item;

      if (reallocation_happened) {
         delete []arr_;
         arr_ = buf;
         capacity_ = capacity;
      }
      ++size_;
   }

   T remove(size_t idx) {
      if (idx >= size_) {
         throw std::out_of_range("incorrect index requested");
      }

      T result{arr_[idx]};

      move_left(arr_ + idx, size_-(idx+1));

      --size_;
      return result;
   }

   const T& get(size_t idx) const {
      if (idx >= size_) {
         throw std::out_of_range("incorrect index requested");
      }
      return arr_[idx];
   }

         T& operator[](std::size_t idx)       { return arr_[idx]; }
   const T& operator[](std::size_t idx) const { return arr_[idx]; }

private:
   T* arr_{ nullptr };
   size_t capacity_;
   size_t size_{0};
};

// ------------------------------------------------------------------
template<typename T, size_t E>
using factor_array = vector_array<T, E, std::multiplies<size_t>>;

// ------------------------------------------------------------------
template<typename T, size_t E>
class matrix_array {
public:
   matrix_array() : capacity_(E) {
      rows_.push_back(new T[E]);
   }

   matrix_array(const matrix_array& other) {
      *this = other;
   }

   matrix_array& operator=(const matrix_array& other) {
      if (this == &other) {
         return *this;
      }

      for (const auto& row: rows_) {
         delete []row;
      }
      rows_.clear();
      rows_.reserve(other.rows_.size());

      for (const auto& other_row: other.rows_) {
         T* row = new T[E];
         copy_range(other_row, row, E);
         rows_.push_back(row);
      }
      size_ = other.size_;
      capacity_ = other.capacity_;

      return *this;
   }

   ~matrix_array() {
      for (const auto& row: rows_) {
         delete []row;
      }
   }

   size_t size() const noexcept {
      return size_;
   }

   size_t capacity() const {
      return capacity_;
   }

   void add_back(const T& item) {
      add(item, size_);
   }

   void add_front(const T& item) {
      add(item, 0);
   }

   void add(const T& item, size_t idx) {
      if (idx > size_) {
         throw std::out_of_range("incorrect index requested");
      }

      auto& capacity = capacity_;
      if (size_ == capacity) {
         capacity += E;
         rows_.push_back(new T[E]);
      }

      const auto row_idx = idx/E;
      const auto col_idx = idx%E;
      auto row = rows_[row_idx];

      auto last_item = row[E-1];
      move_right(row + col_idx, E - (col_idx + 1)); // move current row items to the right

      // move last item to the next row through the all rows
      for (size_t i = row_idx + 1, sz = rows_.size(); i < sz; ++i) {
          auto current_row = rows_[i];
          const auto current_last_item = current_row[E - 1];
          move_right(current_row, E - 1);
          current_row[0] = last_item;
          last_item = current_last_item;
      }
      
      row[col_idx] = item;
      ++size_;
   }

   T remove(size_t idx) {
      if (idx >= size_) {
         throw std::out_of_range("incorrect index requested");
      }

            auto row_idx = idx/E;
      const auto col_idx = idx%E;

      auto target_row = rows_[row_idx];
      T result{target_row[col_idx]};

      // move target row items to the left
      move_left(target_row + col_idx, E-(col_idx+1));
      const auto have_items_in_next_row = [&](){ return (row_idx + 1)*E < size_;};

      while (have_items_in_next_row()) {
         auto next_row = rows_[row_idx+1];
         target_row[E-1] = next_row[0];
         move_left(next_row, E-1);

         target_row = next_row;
         ++row_idx;
      }

      --size_;
      return result;
   }

   const T& get(size_t idx) const {
      if (idx >= size_) {
         throw std::out_of_range("incorrect index requested");
      }
      return rows_[idx/E][idx%E];
   }

         T& operator[](std::size_t idx)       { return rows_[idx/E][idx%E]; }
   const T& operator[](std::size_t idx) const { return rows_[idx/E][idx%E]; }

private:
   size_t capacity_;
   size_t size_{0};
   std::vector<T*> rows_;
};

// ------------------------------------------------------------------
template<typename T>
bool empty(const T& storage) {
   return storage.size() == 0;
}