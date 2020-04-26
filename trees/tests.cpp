#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <bst.hpp>

// ------------------------------------------------------------------
TEST_CASE("base operations", "[binary search tree]") {
   bst::node tree(5);
   tree.insert(4).insert(8).insert(2).insert(6)
       .insert(9).insert(1).insert(3).insert(7);

   CHECK(tree.size() == 9);
   CHECK(bst::is_search_tree(&tree));

   SECTION("remove from head") {
      tree.remove(5);
      CHECK(bst::is_search_tree(&tree));
      CHECK(tree.size() == 8);
      tree.remove(8);
      CHECK(bst::is_search_tree(&tree));
      CHECK(tree.size() == 7);
      tree.remove(9);
      CHECK(bst::is_search_tree(&tree));
      CHECK(tree.size() == 6);
      tree.remove(6);
      CHECK(bst::is_search_tree(&tree));
      CHECK(tree.size() == 5);
      tree.remove(7);
      CHECK(bst::is_search_tree(&tree));
      CHECK(tree.size() == 4);
      tree.remove(4);
      CHECK(bst::is_search_tree(&tree));
      CHECK(tree.size() == 3);
      tree.remove(2);
      CHECK(bst::is_search_tree(&tree));
      CHECK(tree.size() == 2);
      tree.remove(3);
      CHECK(bst::is_search_tree(&tree));
      CHECK(tree.size() == 1);
   }

   SECTION("remove from bst") {
      bst::node tmp(8);
      tmp.insert(5);
      tmp.insert(2);
      tmp.insert(7);
      tmp.insert(6);
      CHECK(bst::is_search_tree(&tmp));
      tmp.remove(5);
      CHECK(bst::is_search_tree(&tmp));

      bst::node tmp1(5);
      tmp1.insert(7);
      tmp1.insert(6);
      tmp1.insert(9);
      tmp1.insert(8);
      CHECK(bst::is_search_tree(&tmp1));
      tmp1.remove(7);
      CHECK(bst::is_search_tree(&tmp1));
   }

   SECTION("search in bst") {
      for (int i = 1; i < 10; ++i) {
         CHECK(tree.search(i));
      }
      CHECK_FALSE(tree.search(0));
      CHECK_FALSE(tree.search(10));
   }

   SECTION("degenerate to list") {
      auto degenerated = bst::node(0);
      for (int i = 1; i < 10; ++i) {
         degenerated.insert(i);
      }
      CHECK(bst::is_search_tree(&degenerated));
      CHECK(degenerated.size() == 10);
      bst::print(&degenerated);

      for (int i = 1; i < 10; ++i) {
         degenerated.remove(i);
         bst::print(&degenerated);
         CHECK(bst::is_search_tree(&degenerated));
      }
      CHECK(degenerated.size() == 1);

      for (int i = -1; i > -10; --i) {
         degenerated.insert(i);
      }
      CHECK(bst::is_search_tree(&degenerated));
      CHECK(degenerated.size() == 10);
      bst::print(&degenerated);

      for (int i = -9; i < 0; ++i) {
         degenerated.remove(i);
      }
      CHECK(bst::is_search_tree(&degenerated));
      CHECK(degenerated.size() == 1);
      bst::print(&degenerated);
   }
}