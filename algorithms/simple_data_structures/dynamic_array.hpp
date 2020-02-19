#pragma once

// ------------------------------------------------------------------
template<typename T>
class array {

public:
    size_t size() const {return size_;}
    const T& operator[](size_t idx) const { return arr_[idx]; }

    virtual void add_back(const T& item)             = 0;
    virtual void add     (const T& item, size_t idx) = 0;
    virtual T    remove  (size_t idx)                = 0;

    virtual ~array() {delete []arr_;}

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
         copy_range(arr+idx, buf+idx+1, sz-idx);
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
       copy_range(arr+idx+1, buf+idx, sz-(idx+1));

       delete []arr;
       arr = buf;
       --sz;
       return result;
    }
};

// ------------------------------------------------------------------
template <typename T>
class vector_array : public array<T> {
   void add_back(const T& item) override {
      add(item, array<T>::size_);
   }

   void add(const T&, size_t idx) override {
      auto& sz = array<T>::size_;
      if (idx > sz) {
         throw std::out_of_range("incorrect index requested");
      }
      auto& arr = array<T>::arr_;
      ++sz;
   }

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
};

// TODO
//class vector_array {};
//class factor_array {};
//class matrix_array {};