#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "external_sort.hpp"

namespace external_sort {
namespace quick {
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

      me.add_file_proxy(file_proxy_t{o_file_name});
   }
   i_file.close();
   return me.merge().name;
}
} // namespace quick
} // namespace external_sort