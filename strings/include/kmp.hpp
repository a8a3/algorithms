#pragma once

#include <string>
#include <vector>

#include "fsm.hpp"

namespace kmp {

using pi_table = std::vector<int>;

// ------------------------------------------------------------------
pi_table build_pi_table_slow(const std::string& pattern) {
   const int p_len = pattern.length();
   pi_table t(p_len);

   for (int p = 0; p < p_len; ++p) {
      for (int i = 1; i <= p; ++i) {
         const auto current = fsm::left(pattern, p + 1);

         const auto prefix = fsm::left(current, i);
         const auto suffix = fsm::right(current, i);

         if (prefix == suffix) {
            t[p] = i;
         }
      }
   }
   return t;
}

// ------------------------------------------------------------------
pi_table build_pi_table_fast(const std::string& pattern) {
   const int p_len = pattern.length();
   pi_table t(p_len);

   for (int p = 1; p < p_len; ++p) {
      int q = t[p - 1];

      while (q > 0 && pattern[p] != pattern[q]) {
         q = t[q-1];
      }

      if (pattern[p] == pattern[q]) {
         ++q;
      }
      t[p] = q;
   }
   return t;
}

// ------------------------------------------------------------------
int search(const std::string& text, const std::string& pattern) {
   const auto pi = build_pi_table_fast(pattern);
   const int p_len = pattern.length();

   int q = 0;
   for (int i = 0, len = text.length(); i < len; ++i) {
      while (q > 0 && text[i] != pattern[q]) {
         q = pi[q-1];
      }
      if (text[i] == pattern[q]) {
         ++q;
      }
      if (q == p_len) {
         return i - p_len + 1;
      }
   }
   return -1;
}

} // namespace kmp