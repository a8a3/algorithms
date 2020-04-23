#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "external_sort.hpp"

namespace external_sort {
namespace bucket {

// ------------------------------------------------------------------
class bucket_t {

public:
   explicit bucket_t(size_t chunk)
   : chunk_(chunk)
   , me_   (chunk) {}

   file_proxy_t merge() {
      if (!values_.empty()) {
         assert(values_.size() == chunk_);
         dump_values_to_file();
      }
      return me_.merge();
   }

   void add_value(uint16_t val) {
      assert(values_.size() <= chunk_);

      if (values_.size() == chunk_) {
         dump_values_to_file();
         values_.clear();
      }
      insert_value(val);
   }

   bool empty() const {
      return values_.empty();
   }

private:
   void dump_values_to_file() {
      std::unique_ptr<uint16_t[]> buf(new uint16_t[chunk_]);
      std::copy(values_.begin(), values_.end(), buf.get());

      const auto o_file_name = generate_file_name(file_idx_++);
      std::ofstream o_file(o_file_name, std::ios::binary | std::ios::out);
      o_file.write(reinterpret_cast<const char*>(buf.get()), chunk_*sizeof(uint16_t));
      o_file.close();

      me_.add_file_proxy(file_proxy_t{o_file_name});
   }

   void insert_value(uint16_t val){
      // insert values in descending order
      if (values_.empty()) {
         values_.emplace_back(val);
         return;
      }

      for (auto it = values_.cbegin(), e = values_.cend(); it != e; ++it) {
         if (val < *it) {
            values_.insert(it, val);
            return;
         }
      }
      values_.emplace_back(val);
   }

   void add_file_proxy(const file_proxy_t& file) {
      me_.add_file_proxy(file);
   }

private:
   const size_t chunk_;
   merge_engine me_;
   std::list<uint16_t> values_;

   static size_t file_idx_;
};
size_t bucket_t::file_idx_ = 0;

// ------------------------------------------------------------------
size_t get_bucket_idx(uint16_t val, size_t bucket_sz) {
   return static_cast<size_t>(val / bucket_sz);
}

// ------------------------------------------------------------------
class buckets_t {

public:
   explicit buckets_t(size_t buckets_count, size_t max_digit_val, size_t chunk)
         : bucket_sz_(std::ceil(static_cast<float>(max_digit_val)/buckets_count))
         , chunk_    (chunk)
   {
      buckets_.reserve(buckets_count);
      for (size_t i = 0; i < buckets_count; ++i) {
         buckets_.emplace_back(chunk_);
      }
   }

   void distribute(uint16_t val) {
      const auto bucket_idx = get_bucket_idx(val, bucket_sz_);
      buckets_[bucket_idx].add_value(val);
   }

   std::string unite() {
      if (buckets_.empty()) return std::string{};

      auto init = buckets_.front().merge().name;
      for (auto it = std::next(buckets_.begin()), e = buckets_.end(); it < e; ++it) {
         init = split_files(init, it->merge().name, chunk_);
      }
      return init;
   }

private:
   std::vector<bucket_t> buckets_;
   const size_t bucket_sz_;
   const size_t chunk_;
};

// ------------------------------------------------------------------
std::string sort_file(const char* file_name, size_t chunk, size_t buckets_count, size_t max_digit_val) {
   const size_t bytes_sz = chunk * sizeof(uint16_t);

   std::ifstream i_file(file_name, std::ios::binary | std::ios::in);

   if (!i_file.is_open()) {
      std::cerr << "unable to open file: " << file_name << '\n';
      return std::string{};
   }
   buckets_t buckets(buckets_count, max_digit_val, chunk);

   std::unique_ptr<uint16_t[]> buf(new uint16_t[chunk]);
   while (!i_file.read(reinterpret_cast<char*>(buf.get()), bytes_sz).eof()) {
      // distribute file stuff to buckets
      for (size_t i = 0; i < chunk; ++ i) {
         buckets.distribute(buf[i]);
      }
   }
   i_file.close();
   return buckets.unite();
}

} // namespace bucket
} // namespace external_sort