#pragma once

#include <array>

namespace graph {

template <size_t N>
using matrix_row = std::array<int, N>;

template<size_t N, size_t M = N>
using matrix = std::array<matrix_row<N>, M>;

using topology = std::vector<std::pair<int, std::vector<int>>>;

} // namespace graph

// ------------------------------------------------------------------
template<size_t N> graph::matrix_row<N> operator+ (graph::matrix_row<N> lhs, const graph::matrix_row<N>& rhs) {
   for (size_t i = 0; i < N; ++i) {
      lhs[i] += rhs[i];
   }
   return lhs;
}

// ------------------------------------------------------------------
template<size_t N> graph::matrix_row<N> operator-(graph::matrix_row<N> lhs, const graph::matrix_row<N>& rhs) {
   for (size_t i = 0; i < N; ++i) {
      lhs[i] -= rhs[i];
   }
   return lhs;
}

// ------------------------------------------------------------------
template <size_t N> std::ostream& operator<< (std::ostream& os, const graph::matrix_row<N>& r) {
   for (const auto i: r) {
      os << i << ' ';
   }
   os << std::endl;
   return os;
}


namespace graph {
// ------------------------------------------------------------------
template<size_t N>
topology demukron(const graph::matrix<N>& m) {
   std::array<bool, N> used = {};

   const auto all_used = [&used] () {
      return std::all_of(used.cbegin(), used.cend(), [](const auto& u){return u;});
   };

   graph::matrix_row<N> sum = {};

   // calc initial sum for all rows
   for (const auto& r: m) {
      sum = sum + r;
   }

   topology result;
   do {
      graph::vertexes vs;
      for (size_t i = 0; i < N; ++i) {
         if (sum[i] == 0 && !used[i]) {
            vs.emplace_back(i);
            used[i] = true;
         }
      }
      // recalc sum
      for(const auto& v: vs) {
         sum = sum - m[v];
      }
      result.emplace_back(result.size(), std::move(vs));
   } while(!all_used());

   return result;
}

} // namespace graph