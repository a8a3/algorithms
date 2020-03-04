#pragma once

#include <functional>
#include <vector>

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
   array(size_t sz = 0) : capacity_(sz) {}

   virtual size_t size     () const noexcept           = 0;
   virtual void   add_back (const T& item)             = 0;
   virtual void   add_front(const T& item)             = 0;
   virtual void   add      (const T& item, size_t idx) = 0;
   virtual T      remove   (size_t idx)                = 0;

   virtual ~array() {delete []arr_;}

   virtual const T& get(size_t idx) const              = 0;
protected:
    T* arr_{ nullptr };
    size_t capacity_;
};

// ------------------------------------------------------------------
template<typename T>
class single_array : public array<T> {
public:

   size_t size() const noexcept override {
      return array<T>::capacity_;
   }

   void add_back(const T& item) override {
      add(item, array<T>::capacity_);
   }

   void add_front(const T& item) override {
      add(item, 0);
   }

   void add(const T& item, size_t idx) override {
      auto& sz = array<T>::capacity_;
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
       auto& sz = array<T>::capacity_;

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
       if (idx >= array<T>::capacity_) {
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
      array<T>::arr_ = new T[array<T>::capacity_];
   }

   size_t size() const noexcept override {
      return size_;
   }

   size_t capacity() const {
      return array<T>::capacity_;
   }

   void add_back(const T& item) override {
      add(item, size_);
   }

   void add_front(const T& item) override {
      add(item, 0);
   }

   void add(const T& item, size_t idx) override {
      if (idx > size_) {
         throw std::out_of_range("incorrect index requested");
      }
      auto capacity = array<T>::capacity_;
      auto buf = array<T>::arr_;

      if (size_ == capacity) {
         capacity = AllocPolicy()(capacity, E);
         buf = new T[capacity];
      }
      const auto reallocation_happened = buf != array<T>::arr_;

      if (reallocation_happened) {
         copy_range(array<T>::arr_, buf, idx);
         copy_range(array<T>::arr_ + idx, buf + (idx+1), size_-idx);
      } else {
         move_right(buf+idx, size_ - idx);
      }
      buf[idx] = item;

      if (reallocation_happened) {
         delete []array<T>::arr_;
         array<T>::arr_ = buf;
         array<T>::capacity_ = capacity;
      }
      ++size_;
   }

   T remove(size_t idx) override {
      if (idx >= size_) {
         throw std::out_of_range("incorrect index requested");
      }

      auto& arr = array<T>::arr_;
      T result{arr[idx]};

      move_left(arr + idx, size_-(idx+1));

      --size_;
      return result;
   }

   const T& get(size_t idx) const override {
      if (idx >= size_) {
         throw std::out_of_range("incorrect index requested");
      }
      return array<T>::arr_[idx];
   }

private:
   size_t size_{0};
};

// ------------------------------------------------------------------
template<typename T, size_t E>
using factor_array = vector_array<T, E, std::multiplies<size_t>>;

// ------------------------------------------------------------------
template<typename T, size_t E>
class matrix_array : public array<T> {
public:
   matrix_array() : array<T>(E) {
      rows_.push_back(new T[E]);
   }

   ~matrix_array() override {
      for (const auto& row: rows_) {
         delete []row;
      }
   }

   size_t size() const noexcept override {
      return size_;
   }

   size_t capacity() const {
      return array<T>::capacity_;
   }

   void add_back(const T& item) override {
      add(item, size_);
   }

   void add_front(const T& item) override {
      add(item, 0);
   }

   void add(const T& item, size_t idx) override {
      if (idx > size_) {
         throw std::out_of_range("incorrect index requested");
      }

      auto& capacity = array<T>::capacity_;
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

   T remove(size_t idx) override {
      if (idx >= size_) {
         throw std::out_of_range("incorrect index requested");
      }

            auto row_idx = idx/E;
      const auto col_idx = idx%E;

//    1 2 3 =>   2 3 4 =>   3 4 5
//    4 5 6      5 6 7      6 7 8
//    7 8 .      8 . .      . . .

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

   const T& get(size_t idx) const override {
      if (idx >= size_) {
         throw std::out_of_range("incorrect index requested");
      }
      return rows_[idx/E][idx%E];
   }

private:
   size_t size_{0};
   std::vector<T*> rows_;
};


