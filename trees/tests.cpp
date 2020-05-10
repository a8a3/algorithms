#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <avl.hpp>
#include <bst.hpp>
#include <rb.hpp>
#include <segment.hpp>

#include <shuffle.hpp>

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
      bst::node degenerated(0);
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
TEST_CASE("avl rebalancing", "[avl tree]") {
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
}


// ------------------------------------------------------------------
TEST_CASE("rb rebalancing", "[red-black tree]") {

   SECTION("ascending insertion") {
      constexpr auto sz = 10;
      rb::tree t;
      for (size_t i = 0; i < sz; ++i) {
         t.insert(i);
      }

      for(size_t i = 0; i < sz; ++i) {
         CHECK(t.search(i));
      }
   }

   SECTION("zig zag insertion") {
      rb::tree t;
      const int arr[] = {100, 80, 90, 85, 87, 86};

      for (int i: arr) {
         t.insert(i);
      }
   }

   SECTION("descending insertion") {
      constexpr auto sz = 10;
      rb::tree t;
      for (int i = sz-1; i >= 0; --i) {
         t.insert(i);
      }

      for(int i = sz-1; i >= 0; --i) {
         CHECK(t.search(i));
      }
   }

   SECTION("left parent, right child") {
      rb::tree t;
      const int values[] = {8, 6, 4, 2, 3};

      for (int i: values) {
         t.insert(i);
      }

      for (int i: values) {
         CHECK(t.search(i));
      }
   }

   SECTION("right parent, left child") {
      rb::tree t;
      const int values[] = {2, 4, 6, 8, 7};

      for (int i: values) {
         t.insert(i);
      }

      for (int i: values) {
         CHECK(t.search(i));
      }
   }

   SECTION("random values insertion") {
      constexpr auto tree_sz = 1000;
      rb::tree t;

      std::unique_ptr<int[]> arr(new int[tree_sz]);
      std::iota(arr.get(), arr.get() + tree_sz, 0);
      make_shuffle(arr.get(), tree_sz);

      for(int i = tree_sz-1; i >= 0; --i) {
         t.insert(arr.get()[i]);
      }
   }
}

// ------------------------------------------------------------------
TEST_CASE("sum calc", "[segments tree]") {
   SECTION("auxiliary") {
      CHECK(segment::next_pow_of2(1) == 1);
      CHECK(segment::next_pow_of2(3) == 4);
      CHECK(segment::next_pow_of2(4) == 4);
      CHECK(segment::next_pow_of2(5) == 8);
      CHECK(segment::next_pow_of2(6) == 8);
      CHECK(segment::next_pow_of2(7) == 8);
      CHECK(segment::next_pow_of2(8) == 8);
   }

   SECTION("calc") {
      constexpr int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
      segment::tree t(arr, sizeof(arr) / sizeof(arr[0]));
      CHECK(t.calc(0, 0) ==  1);
      CHECK(t.calc(0, 7) == 36);
      CHECK(t.calc(0, 1) ==  3);
      CHECK(t.calc(0, 2) ==  6);
      CHECK(t.calc(7, 7) ==  8);
      CHECK(t.calc(6, 7) == 15);
      CHECK(t.calc(5, 7) == 21);
      CHECK(t.calc(7, 8) == 17);
   }

   SECTION("set + calc") {
      segment::tree t(10);
      CHECK(t.calc(0, 9) == 0);
      t.set(0, 10);
      t.set(1, 20);
      CHECK(t.calc(0, 1) == 30);
      t.set(2, 30);
      t.set(3, 40);
      CHECK(t.calc(0, 3) == 100);
      t.set(3, 50);
      CHECK(t.calc(0, 3) == 110);
   }
}