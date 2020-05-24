#pragma once

#include <array>
#include <vector>

namespace graph {

using vertexes = std::vector<int>;

using strong_connected_component = std::vector<vertexes>;

template<size_t N>
using adjacency_vectors = std::array<std::vector<int>, N>;

// ------------------------------------------------------------------
template<size_t N> adjacency_vectors<N> revert(const adjacency_vectors<N>& m) {
      adjacency_vectors<N> res;

   for(size_t r = 0; r < N; ++r) {
      const auto& row = m[r];
      for (size_t c = 0, sz = row.size(); c < sz; ++c) {
         res[row[c]].emplace_back(r);
      }
   }
   return res;
}

// ------------------------------------------------------------------
vertexes revert(const vertexes& v) {
   vertexes reverted(v.size());
   std::reverse_copy(v.begin(), v.end(), reverted.begin());
   return reverted;
}

} // namespace graph

// ------------------------------------------------------------------
template<size_t N> std::ostream& operator<< (std::ostream& os, const graph::adjacency_vectors<N>& m) {
   for (const auto& r: m) {
      for (const auto c: r) {
         os << c << ' ';
      }
      os << ';' << std::endl;
   }
   return os;
}

// ------------------------------------------------------------------
std::ostream& operator<< (std::ostream& os, const graph::vertexes& vs) {
   for (const auto v: vs) {
      os << v << ' ';
   }
   os << std::endl;
   return os;
}

// ------------------------------------------------------------------
std::ostream& operator<< (std::ostream& os, const graph::strong_connected_component& scc) {
   for (const auto& c: scc) {
      os << '[';
      for (const auto i: c) {
         os << i << ' ';
      }
      os << ']' << std::endl;
   }
   return os;
}
