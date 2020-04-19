#pragma once

#include <file.hpp>
#include <merge_sort.hpp>
#include <quick_sort.hpp>

#define TRACE 0

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

//    const auto o_file_sz = file1_sz + file2_sz;
      constexpr auto tmp_file_name = "tmp.bin";
      std::ofstream o_file(tmp_file_name, std::ios::binary | std::ios::out);

      std::unique_ptr<char[]> buf1(new char[bytes_sz]);
      std::unique_ptr<char[]> buf2(new char[bytes_sz]);
      std::unique_ptr<char[]> result_buf(new char[bytes_sz*2]);

      while (!i_file1.read(buf1.get(), bytes_sz).eof() &&
             !i_file2.read(buf2.get(), bytes_sz).eof()) {

         ::merge(reinterpret_cast<uint16_t*>(buf1.get()), chunk_,
                 reinterpret_cast<uint16_t*>(buf2.get()), chunk_, reinterpret_cast<uint16_t*>(result_buf.get()));

         o_file.write(result_buf.get(), bytes_sz*2);
      }

      while (!i_file1.read(buf1.get(), bytes_sz).eof()) {
         i_file1.read(buf1.get(), bytes_sz);
         o_file.write(buf1.get(), bytes_sz);
      }

      while (!i_file2.read(buf1.get(), bytes_sz).eof()) {
         i_file2.read(buf2.get(), bytes_sz);
         o_file.write(buf2.get(), bytes_sz);
      }
      o_file.close();

      print_file_stuff(file1.name, chunk_);
      print_file_stuff(file2.name, chunk_);
      print_file_stuff(tmp_file_name, chunk_);

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
      auto& top = stack_.top();
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
#if TRACE
      print(buf.get(), chunk);
#endif // TRACE

      quick_sort::sort(reinterpret_cast<uint16_t*>(buf.get()), chunk);

      const auto o_file_name = generate_file_name(file_num++);
      std::ofstream o_file(o_file_name, std::ios::binary | std::ios::out);
      o_file.write(buf.get(), bytes_sz);
      o_file.close();

      me.add_file_proxy(file_proxy_t{o_file_name.c_str()});
#if TRACE
      print(buf.get(), chunk);
#endif // TRACE
   }
   i_file.close();
   return me.merge().name;
}