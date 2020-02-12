
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "timer.hpp"

namespace fs = std::filesystem;

//tests/
//     test1.in
//     test1.out
//     test2.in
//     test2.out
//     ...

constexpr auto test_dir_name = "tests";
constexpr auto in_ext  = ".in";
constexpr auto out_ext = ".out";

// ------------------------------------------------------------------
void color_print(const char* str, bool as_red = false) {
   constexpr auto esc = "\x1b";
   constexpr auto red = "[31m";
   constexpr auto green = "[32m";
   constexpr auto old_color = "[0m";

   std::fprintf(stderr, "%s%s%s%s%s", esc, as_red ? red : green, str, esc, old_color);
}

// ------------------------------------------------------------------
int main(int, char**) {
   const auto test_dir_path = fs::current_path() / test_dir_name;

   if (!fs::exists(test_dir_path)) {
      std::cerr << test_dir_name << " directory missing\n";
      return 1;
   }

   for (const auto& p: fs::directory_iterator(test_dir_path)) {

      if (p.path().extension() == in_ext) {
         const auto out_file_name{p.path().stem().string() + out_ext};
         const auto out_file_path{p.path().parent_path() / out_file_name};
         if (!fs::exists(out_file_path)) {
            std::cerr << out_file_name << " file missing\n";
            return 1;
         }

         std::ifstream in(p.path().string());
         std::ifstream out(out_file_path.string());

//         str_len_test slt;
//         in >> slt;
//
//         timer t{"string length test"};
         const auto candidate{""};
//         t.check();

         std::string expected;
         out >> expected;

         if (candidate == expected) {
            const auto str = p.path().filename().string().append(" passed.\n");
            color_print(str.c_str());
         } else {
            const auto str = p.path().filename().string()
                             .append(" failed. expected: ")
                             .append(expected)
                             .append(", got: ")
                             .append(candidate)
                             .append("\n");
            color_print(str.c_str(), true);
         }
      }
   }
   return 0;
}
