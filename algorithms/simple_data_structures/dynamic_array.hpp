#pragma once

// ------------------------------------------------------------------
template<typename T>
class array {

public:
   virtual size_t size    () const                    = 0;
   virtual void   add_back(const T& item)             = 0;
   virtual void   add     (const T& item, size_t idx) = 0;
   virtual T      remove  (size_t idx)                = 0;

   virtual ~array() {delete []arr_;}

   const T& get(size_t idx) const {
     if (idx >= size_) {
        throw std::out_of_range("incorrect index requested");
     }
     return arr_[idx];
   }


   const T& operator[](size_t idx) const { return arr_[idx]; }

protected:
    T* arr_{ nullptr };
    size_t size_{ 0 };
};

// ------------------------------------------------------------------
template <typename T>
void copy_range(const T* src, T* dst, size_t size) {
   for (size_t i = 0; i < size; ++i) {
      *(dst + i) = *(src + i);
   }
}

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

      if (idx == 0) { // add to front
         copy_range(arr, buf + 1, sz);
      } else if (idx == sz) { // add to back
         copy_range(arr, buf, sz);
      } else { // add in middle
         copy_range(arr, buf, idx);
         copy_range(arr+idx, buf+(idx+1), sz-idx);
      }
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
};

// ------------------------------------------------------------------
template <typename T, size_t E = 32>
class vector_array : public array<T> {

public:
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
      auto& total_sz = array<T>::size_;
      auto& arr = array<T>::arr_;

      if (current_size_ == total_sz) {
         T* buf = new T[total_sz+E];
         copy_range(arr, buf, total_sz);
         delete []arr;
         arr = buf;
         total_sz = total_sz+E;
      }
      if (idx == 0) { // add to front
         move_right(arr, current_size_);
      } else if (idx != current_size_){ // add in middle
         move_right(arr+idx, current_size_-idx);
      }
      arr[idx] = item;
      ++current_size_;
   }

   // TODO
   T remove(size_t idx) override {
      auto& sz = array<T>::size_;

      if (idx >= sz) {
         throw std::out_of_range("incorrect index requested");
      }

      auto& arr = array<T>::arr_;

      T result{arr[idx]};
      --sz;
      return result;
   }

private:
   size_t current_size_{0};

   void move_right(T* from, size_t count) {
      for (; count > 0; --count) {
         std::swap(*(from+count), *(from+(count-1)));
      }
   }
};

// TODO
//class vector_array {};
//class factor_array {};
//class matrix_array {};