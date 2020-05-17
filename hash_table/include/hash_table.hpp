#pragma once

#include <memory>

namespace probing {

// ------------------------------------------------------------------
class linear {
public:
   size_t next(size_t prev, size_t, size_t) {
      return ++prev;
   }
   void reset_state() {}
};

// ------------------------------------------------------------------
class quadratic {
public:
   size_t next(size_t prev, size_t, size_t) {
      ++attempt_;
      return prev + attempt_ * attempt_;
   }

   void reset_state() {
      attempt_ = 0;
   }

private:
   size_t attempt_{0};
};

// ------------------------------------------------------------------
class double_hashing {
public:
   size_t next(size_t prev, size_t key, size_t total) {
      ++attempt_;
      return prev + attempt_*(key % (total-1));
   }

   void reset_state() {
      attempt_ = 0;
   }

private:
   size_t attempt_{0};
};

} // probing

// ------------------------------------------------------------------
// open addressing strategy
template<typename Tprobing>
class hash_table {

   struct node {
      node(int k, const std::string& v) : key(k), value(v) {}

      int key{0};
      std::string value;
      bool deleted{false};
   };

public:
   explicit hash_table(size_t sz)
   : size_    (0)
   , capacity_(sz)
   , storage_ (new node_ptr[sz]) {}

   void insert(int key, const std::string& value) {
      if (need_to_rehash()) {
         rehash();
      }

      size_t idx = get_hash(key);

      node_ptr& current = storage_[idx];
      if (current == nullptr) {
         current = std::make_unique<node>(key, value);
      } else {
         if (current->key == key || current->deleted) {
            current->value = value;
         } else {
            // collision detected
            probing_.reset_state();
            do {
               idx = get_hash(probing_.next(idx, key, capacity_));
               node_ptr& next = storage_[idx];

               if (next == nullptr) {
                  next = std::make_unique<node>(key, value);
                  break; // while
               } else if (next->key == key || next->deleted) {
                  next->value = value;
                  break; // while
               }
            } while(true);
         }
      }
      ++size_;
   }

   bool search(int key) const {
      return get_idx_by_key(key) != -1;
   }

   void remove(int key) {
      const int idx = get_idx_by_key(key);

      if (idx == -1) {
         return;
      }
      assert(storage_[idx] != nullptr);
      storage_[idx]->deleted = true;
      --size_;
   }

   size_t size() const {
      return size_;
   }

   size_t capacity() const {
      return capacity_;
   }

private:
   int get_idx_by_key(int key) const {
      size_t idx = get_hash(key);

      node_ptr& current = storage_[idx];
      if (current == nullptr) {
         return -1;
      } else if (!current->deleted && current->key == key) {
         return idx;
      } else {
         // collision detected
         probing_.reset_state();

         int first_deleted = -1;

         do {
            idx = get_hash(probing_.next(idx, key, capacity_));
            node_ptr& next = storage_[idx];

            if (next == nullptr) {
               break; // while
            } else {
               if (next->key == key) {
                  if (next->deleted) {
                     return -1;
                  } else {
                     if (first_deleted == -1) {
                        return idx;
                     } else {
                        // swap first deleted and found
                        storage_[first_deleted].swap(storage_[idx]);
                        return first_deleted;
                     }
                  }
               } else {
                  if (first_deleted == -1 && next->deleted) {
                     first_deleted = idx;
                  }
               }
            }
         } while(true);
         return -1;
      }
   }

   void rehash() {
      const size_t old_capacity = capacity_;
      auto old_storage = std::move(storage_);

      capacity_ <<= 1;
      storage_.reset(new node_ptr[capacity_]);
      size_ = 0;

      for (size_t i = 0; i < old_capacity; ++i) {
         node_ptr& n = old_storage[i];
         if (n && !n->deleted) {
            insert(n->key, n->value);
         }
      }
   }

   bool need_to_rehash() const {
      return size_ >= (capacity_ >> 1);
   }

   size_t get_hash(int key) const {
      return key % capacity_;
   }

   mutable Tprobing probing_;
   size_t size_;
   size_t capacity_;

   using node_ptr = std::unique_ptr<node>;
   std::unique_ptr<node_ptr[]> storage_;
};
