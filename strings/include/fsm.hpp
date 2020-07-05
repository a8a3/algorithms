#pragma once

#include <string>
#include <vector>

namespace fsm {
using table = std::vector<std::vector<int>>;

// ------------------------------------------------------------------
std::string left(const std::string& s, size_t idx) {
   return s.substr(0, idx);
}

// ------------------------------------------------------------------
std::string right(const std::string& s, size_t idx) {
   const auto len = s.length();
   return s.substr(len-idx);
}

// ------------------------------------------------------------------
table build_table(const std::string& pattern) {

   constexpr auto alphabet_len = 2; // AB
   const int p_len = pattern.length();
   table t;

   for (int p = 0; p < p_len; ++p) {
      t.emplace_back(alphabet_len);

      for (char a = 'A'; a <= 'B'; ++a) {
         const auto candidate = left(pattern, p) + a;
         int len = p + 1;
         while (left(pattern, len) != right(candidate, len)) {
            --len;
         }
         t[p][a-'A'] = len;
      }
   }
   return t;
}

// ABAAABAB
// ABAB

// ------------------------------------------------------------------
int search(const std::string& text, const std::string& pattern) {
   const auto table = build_table(pattern);
   int p_len = pattern.length();
   int state = 0;

   for(int i = 0, len = text.length(); i < len; ++i) {
      state = table[state][text[i]-'A'];
      if (state == p_len) {
         return i - (p_len-1);
      }
   }

   return -1;
}

} // fsm

