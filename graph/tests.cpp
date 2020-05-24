#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <dfs.hpp>
#include <kosaraju.hpp>
#include <graph.hpp>
#include <topological_sort.hpp>

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

// ------------------------------------------------------------------
TEST_CASE("Demukron", "[graph]") {

   SECTION("common functions") {
      graph::matrix_row<4> r1 = {2, 2, 2, 2};
      graph::matrix_row<4> r2 = {4, 1, 1, 4};
      graph::matrix_row<4> r3= r1 + r2;

      CHECK(r3 == graph::matrix_row<4>{6, 3, 3, 6});
      CHECK(r3-r2 == r1);
   }

   SECTION("Demukron_1") {
      graph::matrix<10, 10> m = {{
//     0  1  2  3  4  5  6  7  8  9
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},  // 0
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},  // 1
      {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},  // 2
      {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},  // 3
      {0, 0, 0, 1, 0, 0, 0, 0, 0, 1},  // 4
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 5
      {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},  // 6
      {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},  // 7
      {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},  // 8
      {0, 0, 0, 0, 0, 0, 0, 0, 1, 0}   // 9
      }};
      const auto topology = graph::demukron(m);
      CHECK(topology.size() == 4);
   }

   SECTION("Demukron_2") {
      graph::matrix<14, 14> m = {{
//     1  2  3  4  5  6  7  8  9  10 11 12 13 14
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},  //  1
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0},  //  2
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //  3
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  //  4
      {0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0},  //  5
      {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0},  //  6
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},  //  7
      {0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},  //  8
      {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},  //  9
      {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},  // 10
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 11
      {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 12
      {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // 13
      {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}   // 14
      }};
      const auto topology = graph::demukron(m);
      CHECK(topology.size() == 6);
   }
}