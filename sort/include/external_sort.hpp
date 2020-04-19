#pragma once

#include <fstream>
#include <stack>

#include <merge_sort.hpp>
#include <shuffle.hpp>
#include <quick_sort.hpp>

// ------------------------------------------------------------------
void print_buf(uint16_t arr[], size_t sz) {
   for (size_t i = 0; i < sz; ++i) {
      std::clog << arr[i] << ' ';
   }
   std::clog << '\n';
}

// ------------------------------------------------------------------
void print_file_stuff(const std::string& name, size_t chunk) {
   const size_t bytes_sz = chunk * sizeof(uint16_t);

   std::ifstream file(name, std::ios::binary | std::ios::in);

   if (!file.is_open()) {
      std::cerr << "unable to open file: " << name << '\n';
      return;
   }

   std::clog << "file " << name << ":" << '\n';
   std::unique_ptr<char[]> buf(new char[bytes_sz]);

   while(!file.read(buf.get(), bytes_sz).eof()) {
      print_buf(reinterpret_cast<uint16_t*>(buf.get()), chunk);
   }
   file.close();
}

// ------------------------------------------------------------------
void create_shuffled_binary_file(const char* name, size_t count, size_t write_by) {
   std::ofstream file(name, std::ios::binary | std::ios::out);

   if (!file.is_open()) {
      std::cerr << "unable to open file: " << name << '\n';
      return;
   }
   size_t writed = 0;

   while (writed < count) {

      if (writed + write_by > count) {
         write_by = count - writed;
      }

      std::unique_ptr<uint16_t []> buf(new uint16_t[write_by]);
      std::iota(buf.get(), buf.get() + write_by, writed);
      make_shuffle(buf.get(), write_by);
      file.write(reinterpret_cast<const char*>(buf.get()), write_by * sizeof(uint16_t));
      writed += write_by;
   }
   file.close();
}

// ------------------------------------------------------------------
std::string generate_file_name(size_t num) {
   return std::to_string(num).append(".bin");
}

// ------------------------------------------------------------------
struct file_proxy_t {
   std::string name;
   size_t generation{0};
};

// ------------------------------------------------------------------
class merge_engine {
   static constexpr auto tmp_file_name = "tmp.bin";
   std::stack<file_proxy_t> stack_;
   const size_t chunk_;

   file_proxy_t merge_files(const file_proxy_t& file1, const file_proxy_t& file2) const {
      const size_t bytes_sz = chunk_*sizeof(uint16_t);

      std::ifstream i_file1(file1.name, std::ios::binary | std::ios::in);
      if (!i_file1.is_open()) {
         std::cerr << "unable to open file: " << file1.name << '\n';
         return file_proxy_t{};
      }

      std::ifstream i_file2(file2.name, std::ios::binary | std::ios::in);
      if (!i_file2.is_open()) {
         std::cerr << "unable to open file: " << file2.name << '\n';
         return file_proxy_t{};
      }

      std::ofstream o_file(tmp_file_name, std::ios::binary | std::ios::out);

      std::unique_ptr<char[]> b1(new char[bytes_sz]);
      std::unique_ptr<char[]> b2(new char[bytes_sz]);
      std::unique_ptr<char[]> rb(new char [bytes_sz]);

      uint16_t* buf1 = reinterpret_cast<uint16_t*>(b1.get());
      uint16_t* buf2 = reinterpret_cast<uint16_t*>(b2.get());
      uint16_t* result_buf = reinterpret_cast<uint16_t*>(rb.get());

      size_t r1{0};
      size_t r2{0};
      size_t  w{0};

      i_file1.read(reinterpret_cast<char*>(buf1), bytes_sz);
      i_file2.read(reinterpret_cast<char*>(buf2), bytes_sz);

      while (true) {
         while(r1 < chunk_ && r2 < chunk_ && w < chunk_) {
            if (buf1[r1] < buf2[r2]) {
               result_buf[w] = buf1[r1];
               ++r1;
            } else {
               result_buf[w] = buf2[r2];
               ++r2;
            }
            ++w;
         }

         if (w == chunk_) {
            o_file.write(rb.get(), bytes_sz);
            w = 0;
         }

         if (r1 == chunk_) {
            if (i_file1.read(b1.get(), bytes_sz).eof())
               break; // while
            r1 = 0;
         }

         if (r2 == chunk_) {
            if (i_file2.read(b2.get(), bytes_sz).eof())
               break; // while
            r2 = 0;
         }
      }

      if (r1 < chunk_) {
         while (r1 < chunk_ && w < chunk_) {
            result_buf[w] = buf1[r1];
            ++r1;
            ++w;
         }
         o_file.write(rb.get(), bytes_sz);
      }

      if (r2 < chunk_) {
         while (r2 < chunk_ && w < chunk_) {
            result_buf[w] = buf2[r2];
            ++r2;
            ++w;
         }
         o_file.write(rb.get(), bytes_sz);
      }

      while (!i_file1.read(b1.get(), bytes_sz).eof()) {
         o_file.write(b1.get(), bytes_sz);
      }

      while (!i_file2.read(b2.get(), bytes_sz).eof()) {
         o_file.write(b2.get(), bytes_sz);
      }
      o_file.close();


      std::remove(file1.name.c_str());
      std::remove(file2.name.c_str());

      std::rename(tmp_file_name, file1.name.c_str());
      return file_proxy_t{file1.name, file1.generation + 1};
   }

public:
   explicit merge_engine(size_t chunk) : chunk_(chunk) {}

   void add_file_proxy(const file_proxy_t& file) {
      if (stack_.empty() || stack_.top().generation != file.generation) {
         stack_.push(file);
         return;
      }
      auto top = std::move(stack_.top());
      stack_.pop();
      const auto merged = merge_files(top, file);
      add_file_proxy(merged);
   }

   file_proxy_t merge() {
      file_proxy_t res = stack_.top();
      stack_.pop();

      while (!stack_.empty()) {
         res = merge_files(res, stack_.top());
         stack_.pop();
      }
      return res;
   }
};

// ------------------------------------------------------------------
std::string sort_file(const char* file_name, size_t chunk) {
   const size_t bytes_sz = chunk * sizeof(uint16_t);
   std::ifstream i_file(file_name, std::ios::binary | std::ios::in);

   if (!i_file.is_open()) {
      std::cerr << "unable to open file: " << file_name << '\n';
      return std::string{};
   }

   merge_engine me(chunk);

   size_t file_num = 0;
   std::unique_ptr<char[]> buf(new char[bytes_sz]);

   while (!i_file.read(buf.get(), bytes_sz).eof()) {
      quick_sort::sort(reinterpret_cast<uint16_t*>(buf.get()), static_cast<int>(chunk));

      const auto o_file_name = generate_file_name(file_num++);
      std::ofstream o_file(o_file_name, std::ios::binary | std::ios::out);
      o_file.write(buf.get(), bytes_sz);
      o_file.close();

      me.add_file_proxy(file_proxy_t{o_file_name.c_str()});
   }
   i_file.close();
   return me.merge().name;
}