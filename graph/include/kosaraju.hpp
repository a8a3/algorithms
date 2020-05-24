#pragma once

#include "graph.hpp"

namespace graph {

// ------------------------------------------------------------------
template<size_t N> strong_connected_component kosaraju(const adjacency_vectors<N>& m) {
   // revert input graph
   const auto reverted = graph::revert(m);

   // collect all vertexes via dfs
   const auto d = graph::force_dfs(reverted);

   // revert result
   const auto rd = graph::revert(d);

   // collect strong connected components in order of result above
   return graph::kosaraju_dfs(m, rd);
}

} // namespace graph