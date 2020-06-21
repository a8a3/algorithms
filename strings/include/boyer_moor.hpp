
#include <string>

namespace strings {

// ------------------------------------------------------------------
int simple_search(const std::string& text, const std::string& pattern) {
   const int t_sz = text.size();
   const int p_sz = pattern.size();

   for(int t = 0, sz = t_sz-p_sz; t <= sz; ++t) {
      int p = 0;
      for (;p < p_sz; ++p) {
         if (text[p+t] != pattern[p]) {
            break; // for
         }
      }
      if (p == p_sz) {
         return t;
      }
   }
   return -1;
}

// ------------------------------------------------------------------
std::vector<int> build_prefix(const std::string& pattern) {
   constexpr auto alphabet_sz = 256;
   std::vector<int> prefixes(alphabet_sz);
   std::fill(prefixes.begin(), prefixes.end(), -1);

   // add all symbols except last one
   for(size_t i = 0, sz = pattern.size() - 1; i < sz; ++i) {
      prefixes[pattern[i]] = i;
   }

   return prefixes;
}

// ------------------------------------------------------------------
int boyer_moor_horspool(const std::string& text, const std::string& pattern) {
   const auto prefixes = build_prefix(pattern);

   // search
   int t_sz = text.size();
   int p_sz = pattern.size();
   int last_idx = pattern.size()-1;

   int t = 0, sz = t_sz-p_sz;
   while(t <= sz) {

      int p = last_idx;
      while(p >= 0 && pattern[p] == text[t+p]) {
         --p;
      }
      if (p == -1) {
         return t; // pattern is matched
      }
      const int idx = prefixes[text[t+last_idx]];

      if (idx == -1) {
         // symbol is not presented in pattern, move pattern to symbol position
         t += p + 1;
      } else {
         t += last_idx - idx;
      }
   }
   return -1;
}

// ------------------------------------------------------------------
std::vector<int> build_suffix(const std::string& pattern) {
   if (pattern.empty()) {
      return {};
   }

   const int p_len = pattern.length();
   std::vector<int> tbl(p_len + 1);
   std::fill(tbl.begin(), tbl.end(), p_len);
   tbl[p_len] = 1; // empty suffix

   for (int i = p_len-1; i >= 0; --i) {

      const std::string suffix = pattern.substr(i);

      for (int j = i - 1; j >= 0; j--) {
         // try to find fully matched suffix
         const std::string candidate = pattern.substr(j, suffix.length());

         if (candidate == suffix) {
            tbl[p_len - 1 - i] = p_len - j - candidate.length();
            break; // for
         }
      }

      if (tbl[p_len - 1 - i] == p_len) {
         // prefix was not found, try to find subprefixes from begining of the pattern
         for (int s = i + 1; s < p_len; ++s) {

            const std::string sub_suffix = pattern.substr(s);
            const std::string candidate = pattern.substr(0, sub_suffix.length());
            if (candidate == sub_suffix) {
               tbl[p_len - 1 - i] = p_len - candidate.length();
               break; // for
            }
         }
      }
   }
   return tbl;
}

// ------------------------------------------------------------------
int boyer_moor(const std::string& text, const std::string& pattern) {
   const auto prefixes = build_prefix(pattern);
   const auto suffixes = build_suffix(pattern);

   // search
   int t_sz = text.size();
   int p_sz = pattern.size();
   int last_idx = pattern.size()-1;

   int t = 0, sz = t_sz-p_sz;
   while(t <= sz) {

      int p = last_idx;
      while(p >= 0 && pattern[p] == text[t+p]) {
         --p;
      }
      if (p == -1) {
         return t; // pattern is matched
      }
      const int pref_idx = prefixes[text[t+last_idx]];
      const int suff_idx = suffixes[p+1];

      if (pref_idx == -1) {
         // symbol is not presented in pattern, move pattern to symbol position
         t += p + 1;
      } else {
         t += std::max(last_idx - pref_idx, last_idx - suff_idx);
      }
   }
   return -1;
}

} // namespace strings