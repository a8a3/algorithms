#pragma once

#include <graph.hpp>


namespace {

// ------------------------------------------------------------------
template<size_t N> void dfs_helper
(
   const graph::adjacency_vectors<N>& am,
   size_t current_vertex_idx,
   graph::vertexes& result,
   std::array<bool, N>& visited
) {
   assert(current_vertex_idx < N);
   if (visited[current_vertex_idx]) {
      return;
   }
   visited[current_vertex_idx] = true;

   const auto& adjacent_vertexes = am[current_vertex_idx];
   for (const auto v: adjacent_vertexes) {
      dfs_helper(am, v, result, visited);
   }
   result.emplace_back(current_vertex_idx);
}

} // namespace

namespace graph {

// ------------------------------------------------------------------
template<size_t N>
vertexes dfs(const adjacency_vectors<N>& am) {
   std::array<bool, N> visited = {};
   vertexes result;
   dfs_helper(am, 0, result, visited);
   return result;
}

// ------------------------------------------------------------------
template<size_t N>
vertexes force_dfs(const adjacency_vectors<N>& am) {
   std::array<bool, N> visited = {};
   vertexes result;
   for (size_t i = 0, sz = am.size(); i < sz; ++i) {
      dfs_helper(am, i, result, visited);
   }
   return result;
}

// ------------------------------------------------------------------
template<size_t N>
std::vector<vertexes> kosaraju_dfs(const adjacency_vectors<N>& am, const vertexes& v) {
   std::array<bool, N> visited = {};
   strong_connected_component result;

   for (const auto i: v) {
      vertexes component;
      dfs_helper(am, i, component, visited);
      if (!component.empty()) {
         result.emplace_back(std::move(component));
      }
   }
   return result;
}

} // namespace graph