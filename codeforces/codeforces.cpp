#include <iostream>

// ----------------------------------------------
int main() {

   int t;
   std::cin >> t;

   while(t--) {
      int e;
      std::cin >> e;
      int sz = e;

      int* inexp = new int[sz+2];
      for (int i = 0; i < sz + 2; ++i) {
         inexp[i] = 0;
      }

      while(e--) {
         int ie;
         std::cin >> ie;
         ++inexp[ie];
      }

      int groups = 0;
      for (int i = 1; i < sz+1; ++i) {
         int ie = inexp[i];
         groups += ie / i;
         inexp[i+1] += ie % i;
      }
      std::cout << groups << '\n';
      delete[] inexp;
   }
}