#pragma once

#include <algorithm>
#include <array>

namespace graph {

#if 0

   0*      1*
   |
   2*      3*
     \
       4*

v p
0 0
1 1
2 0
3 3
4 0


#endif



// ------------------------------------------------------------------
template<size_t N>
class disjoint_sets {

public:
   disjoint_sets() {
      // parents -> 0, 1, 2, ... N
      std::generate(parents_.begin(), parents_.end(), [i=0]()mutable{return i++;});
   }

   // v -> u
   bool merge(int u, int v) {
      if (parents_[u] == parents_[v]) {
         // vertexes belong to one component
         return false;
      }

      // otherwise change parents for:
      // -v
      // -for others who have same parent
      const int prev_v_parent = parents_[v];
      const int new_v_parent = parents_[u];

      parents_[v] = new_v_parent;

      std::for_each(parents_.begin(), parents_.end(),
      [prev_v_parent, new_v_parent](auto& p) {
         if (p == prev_v_parent) {
            p = new_v_parent;
         }
      });
      return true;
   }

private:
   std::array<int, N> parents_;
};



} // namespace graph
