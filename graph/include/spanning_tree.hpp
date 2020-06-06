#pragma once

#include <algorithm>
#include <array>
#include <vector>
#include "union_find.hpp"

namespace graph {

// ------------------------------------------------------------------
struct weighted_vertex_t {
   int vertex{0};
   int weight{0};
};

using weighted_vertexes = std::vector<weighted_vertex_t>;

template<size_t N>
using weighted_adjacency_vectors = std::array<weighted_vertexes, N>;


// ----------------------------------------------------------------
struct weighted_edge{
   int x{0};
   int y{0};
   int weight{0};
};
using weighted_edges = std::vector<weighted_edge>;

// ------------------------------------------------------------------
int get_weight(const weighted_edges& e) {
   return std::accumulate(e.begin(), e.end(), 0, [](int w, const auto& item) {return w + item.weight;});
}

// ------------------------------------------------------------------
template<size_t N>
bool has_cycle_helper(const weighted_adjacency_vectors<N>& wav, std::vector<bool>& visited, int idx, int parent) {

   visited[idx] = true;

   const auto& adj = wav[idx];
   for (int i = 0, sz = adj.size(); i < sz; ++i) {
      if (visited[adj[i].vertex]) {
         if (adj[i].vertex != parent) {
            return true;
         }
      } else {
         if (has_cycle_helper(wav, visited, adj[i].vertex, idx)) {
            return true;
         }
      }
   }
   return false;
}

// ------------------------------------------------------------------
template<size_t N>
bool has_cycle(const weighted_adjacency_vectors<N>& wav) {
   std::vector<bool> visited(N);

   for (int i = 0; i < static_cast<int>(N); ++i) {
      if (!visited[i]) {
         if (has_cycle_helper(wav, visited, i, -1)) {
            return true;
         }
      }
   }
   return false;
}



// ------------------------------------------------------------------
template<size_t N> weighted_edges boruvkas_algorithm(const weighted_adjacency_vectors<N>& wav) {
   // transform weighted vertexes to edges
   weighted_edges src;
   std::for_each(wav.begin(), wav.end(),
      [i=0, &src](const auto& vec) mutable {
         std::for_each(vec.begin(), vec.end(),
            [&i, &src](const auto& v) {
               src.emplace_back(weighted_edge{i, v.vertex, v.weight});
         });
      ++i;
   });

   // sort edges by weight
   std::sort(src.begin(), src.end(), [](const auto& lhs, const auto& rhs) {return lhs.weight < rhs.weight;});

   graph::disjoint_sets<N> ds;

   weighted_edges result;
   result.reserve(N-1);

   for(size_t i = 0, sz = src.size(); i < sz && result.size() < N-1; ++i) {
      auto& e = src[i];

      if (ds.merge(e.x, e.y)) {
         result.emplace_back(std::move(e));
      }
   }

   return result;
}

// ------------------------------------------------------------------
template <size_t N> void add_edge(weighted_adjacency_vectors<N>& to, const weighted_edge& e) {
   to[e.x].emplace_back(weighted_vertex_t{e.y, e.weight});
   to[e.y].emplace_back(weighted_vertex_t{e.x, e.weight});
}

// ------------------------------------------------------------------
template <size_t N> void remove_last_edge(weighted_adjacency_vectors<N>& from, const weighted_edge& e) {
   from[e.x].erase(std::prev(from[e.x].end()));
   from[e.y].erase(std::prev(from[e.y].end()));
}

// ------------------------------------------------------------------
template<size_t N> weighted_edges kruskals_algorithm(const weighted_adjacency_vectors<N>& wav) {
   // transform weighted vertexes to edges
   weighted_edges src;
   std::for_each(wav.begin(), wav.end(),
   [i=0, &src](const auto& vec) mutable {
      std::for_each(vec.begin(), vec.end(),
         [&i, &src](const auto& v) {
            src.emplace_back(weighted_edge{i, v.vertex, v.weight});
         });
      ++i;
   });

   // sort edges by weight
   std::sort(src.begin(), src.end(), [](const auto& lhs, const auto& rhs) {return lhs.weight < rhs.weight;});

   weighted_adjacency_vectors<N> cycles_checker;

   weighted_edges result;
   result.reserve(N-1);

   for(size_t i = 0, sz = src.size(); i < sz && result.size() < N-1; ++i) {
      auto& e = src[i];
      add_edge(cycles_checker, e);

      if (has_cycle(cycles_checker)) {
         remove_last_edge(cycles_checker, e);
      } else {
         result.emplace_back(std::move(e));
      }
   }
   return result;
}


} // namespace graph