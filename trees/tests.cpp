#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <bst.hpp>
#include <avl.hpp>

// ------------------------------------------------------------------
TEMPLATE_TEST_CASE("base operations", "[binary search tree][template]", bst::node, avl::node) {
   TestType tree(5);
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

   SECTION("remove") {
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

   SECTION("search") {
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

// ------------------------------------------------------------------
TEST_CASE("height detection test", "[avl tree]") {
   avl::node tree(42);

   SECTION("rebalancing, small left rotation") {
      CHECK(tree.get_height() == 0);
      tree.insert(64);
      tree.insert(128);
      CHECK(tree.get_height() == 2);
      tree.insert(256);
      CHECK(tree.get_height() == 2);
      CHECK(bst::is_search_tree(&tree));
   }

   SECTION("rebalancing, small right rotation") {
      CHECK(tree.get_height() == 0);
      tree.insert(40);
      tree.insert(38);
      CHECK(tree.get_height() == 2);
      tree.insert(36);
      CHECK(tree.get_height() == 2);
      tree.insert(34);
      tree.insert(32);
      tree.insert(30);
      CHECK(bst::is_search_tree(&tree));
   }

   SECTION("rebalancing, big left rotation") {
      CHECK(tree.get_height() == 0);
      tree.insert(48);
      tree.insert(36);
      tree.insert(32);
      tree.insert(38);
      tree.insert(37);
      CHECK(tree.get_height() == 2);
      tree.insert(39);
      CHECK(tree.get_height() == 2);
      CHECK(bst::is_search_tree(&tree));
   }

   SECTION("rebalancing, big right rotation") {
      CHECK(tree.get_height() == 0);
      tree.insert(40);
      tree.insert(48);
      tree.insert(52);
      tree.insert(46);
      tree.insert(47);
      CHECK(tree.get_height() == 2);
      tree.insert(45);
      CHECK(tree.get_height() == 2);
      CHECK(bst::is_search_tree(&tree));
   }

   SECTION("rebalancing, add a lot of nodes") {
      for (int i = 0; i < 10'000; ++i) {
         tree.insert(i);
      }
//    bst::print_by_level(&tree);
   }
}