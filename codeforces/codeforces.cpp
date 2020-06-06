#include <iostream>


int main() {
   int t;
   std::cin >> t;

   while(t--) {
      int c, j, p;
      std::cin >> c >> j >> p;
      int cp = c/p;

      int res = 0;

      if (j == 0 || j < cp) {
         res = j;
      } else if (j > cp && j < p*(cp-1)+1) {
         res = cp - ((j-cp)/(p-1) + (j-cp)%(p-1));
      }

      std::cout << res << '\n';
   }
}