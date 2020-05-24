#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <dfs.hpp>
#include <kosaraju.hpp>
#include <graph.hpp>

// ------------------------------------------------------------------
TEST_CASE("common operations", "[graph]") {

   SECTION("dfs") {
      graph::adjacency_vectors<9> g{{
         {1, 2},  // 0
         {3, 4},  // 1
         {5, 6},  // 2
         {7},     // 3
         {},      // 4
         {},      // 5
         {},      // 6
         {8},     // 7
         {}       // 8
      }};
      CHECK(graph::dfs(g) == graph::vertexes{8, 7, 3, 4, 1, 5, 6, 2, 0});
   }

   SECTION("single revert") {
      graph::adjacency_vectors<3> g{{
         {1, 2},  // 0
         {},      // 1
         {}       // 2
      }};
      CHECK(graph::revert(g) == graph::adjacency_vectors<3>{{{}, {0}, {0}}});
   }

   SECTION("double revert") {
      graph::adjacency_vectors<9> g{{
         {1, 2},  // 0
         {3, 4},  // 1
         {5, 6},  // 2
         {7},     // 3
         {},      // 4
         {},      // 5
         {},      // 6
         {8},     // 7
         {}       // 8
      }};
      const auto reverted = graph::revert(g);
      CHECK(graph::revert(reverted) == g);
   }
}

// ------------------------------------------------------------------
TEST_CASE("Kosaraju", "[graph]") {

   SECTION("Kosaraju's algorithm") {
      graph::adjacency_vectors<8> g1{{
         {1},        // 0
         {2, 4, 5},  // 1
         {3, 6},     // 2
         {2, 7},     // 3
         {0, 5},     // 4
         {6},        // 5
         {5},        // 6
         {3, 6}      // 7
      }};

      const auto scc1 = graph::kosaraju(g1);
      CHECK(scc1.size() == 3);

      graph::adjacency_vectors<8> g2{{
         {4, 5, 6},  // 0
         {0, 3, 5},  // 1
         {6, 7},     // 2
         {},         // 3
         {},         // 4
         {},         // 5
         {},         // 6
         {3, 6}      // 7
      }};

      const auto scc2 = graph::kosaraju(g2);
      CHECK(scc2.size() == 8);
   }
}
