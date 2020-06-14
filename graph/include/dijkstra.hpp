#pragma once

#include "spanning_tree.hpp"

#include <algorithm>
#include <climits>

namespace graph {

template<size_t N>
weighted_edges dijkstras_algorithm(const weighted_adjacency_vectors<N>& wav) {
   std::vector<bool> visited(N);

   // x(origin) y(parent) weight
   weighted_edges we(N);
   std::generate(we.begin(), we.end(),
   [n = 0]() mutable {
      auto res = weighted_edge{n, n, INT_MAX};
      ++n;
      return res;
   });
   we[0].weight = 0;

   const auto get_min_weighted_vertex_idx = [&visited, &we](){
      int min_idx = 0;
      int min_w = INT_MAX;
      for(int i = 0, sz = we.size(); i < sz; ++i) {
         if (!visited[we[i].x]) {
            if (min_w > we[i].weight) {
               min_w = we[i].weight;
               min_idx = i;
            }
         }
      }
      return min_idx;
   };

   // vertexes
   int cnt = N;
   while(cnt--) {
      int current_idx = get_min_weighted_vertex_idx();
      if (visited[current_idx]) {
         continue;
      }
      const auto& current_vec = wav[current_idx];

      // adjacent vertexes
      for(const auto& v: current_vec) {
         if (!visited[v.vertex]) {
            int current_weight = we[current_idx].weight + v.weight;

            if (we[v.vertex].weight > current_weight) {
               we[v.vertex].weight = current_weight;
               we[v.vertex].y = current_idx;
            }
         }
      }
      visited[current_idx] = true;
   }
   return we;
}

// ------------------------------------------------------------------
std::pair<std::string, int> get_path_to(const weighted_edges& we, int idx) {
   std::string path = std::to_string(idx);
   int parent = idx;

   while (parent != 0) {
      path = std::to_string(we[parent].y) + path;
      parent = we[parent].y;
   }
   return {path, we[idx].weight};
}

} // namespace graph