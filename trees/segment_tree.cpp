#include <segment.hpp>
#include <fstream>
#include <iostream>

int main() {

   std::ifstream in("sum.in");
   std::ofstream out("sum.out");

   int N, K, a, b;
   std::string action;

   in >> N >> K;

   segment::tree t(N);

   while(K > 0) {
      in >> action >> a >> b;
      if (action == "A") {
         t.set(a, b);
      } else if (action == "Q") {
         out << t.calc(a, b) << '\n';
      }
      --K;
   }

   in.close();
   out.close();
   return 0;
}