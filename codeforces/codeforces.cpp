#include <cassert>
#include <iostream>
#include <vector>

// wr
// abcbcc

// ------------------------------------------------------------------
size_t remove_duplicates(char* s, size_t sz) {
   if (sz < 2) {
      return sz;
   }
   size_t r = 1;
   size_t w = 0;

   while (r < sz) {
      if (s[r] != s[w]) {
         ++w;
         if (w != r) {
            s[w] = s[r];
         }
      }
      ++r;
   }
   return w;
}

// ------------------------------------------------------------------
int main() {

   using test = std::pair<std::string, size_t>;
   std::vector<test> data{
      {"aaabbb", 1},
      {"aaaaaa", 0},
      {"abcdef", 5},
      {"abbbbb", 1},
      {"aaaaab", 1}
   };

   for (auto& t: data) {
      assert(remove_duplicates(&t.first.front(), t.first.size()) == t.second);
   }
}