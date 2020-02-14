
#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "timer.hpp"

namespace fs = std::filesystem;

namespace files_tester {

//tests/
//     test1.in
//     test1.out
//     test2.in
//     test2.out
//     ...

namespace {

constexpr auto test_dir_name{"tests"};
constexpr auto in_ext{".in"};
constexpr auto out_ext{".out"};


// ------------------------------------------------------------------
void color_print(const char *str, bool as_red) {
   constexpr auto esc{"\x1b"};
   constexpr auto red{"[31m"};
   constexpr auto green{"[32m"};
   constexpr auto old_color{"[0m"};

   std::fprintf(stderr, "%s%s%s%s%s", esc, as_red ? red : green, str, esc, old_color);
}

// ------------------------------------------------------------------
void red_print(const char* str) {
   color_print(str, true);
}

// ------------------------------------------------------------------
void green_print(const char* str) {
   color_print(str, false);
}

} // empty namespace

// ------------------------------------------------------------------
template<typename T>
bool check() {
   const auto test_dir_path = T::get_test_dir();

   if (!fs::exists(test_dir_path)) {
      std::cerr << test_dir_name << " directory missing\n";
      return false;
   }

   for (const auto &p: fs::directory_iterator(test_dir_path)) {

      if (p.path().extension() == in_ext) {
         const auto out_file_name{p.path().stem().string() + out_ext};
         const auto out_file_path{p.path().parent_path() / out_file_name};

         if (!fs::exists(out_file_path)) {
            std::cerr << out_file_name << " file missing\n";
            return false;
         }

         std::ifstream in(p.path().string());
         std::ifstream out(out_file_path.string());

         std::string expected;
         out >> expected;

         T test;
         in >> test;

         if (test.execute(expected)) {
            green_print(p.path().filename().string().append(" passed.\n").c_str());
         } else {
            red_print(p.path().filename().string().append(" failed.\n").c_str());
         }
      }
   }
   return true;
}

} // namespace files_tester