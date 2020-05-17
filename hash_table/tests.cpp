#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>

#include <hash_table.hpp>

// ------------------------------------------------------------------
TEMPLATE_TEST_CASE("base operations", "[hash_table]", probing::linear, probing::quadratic, probing::double_hashing) {

   SECTION("rehash") {
      hash_table<TestType> t(8);
      CHECK(t.capacity() == 8);
      t.insert(1, "one");
      t.insert(2, "two");
      t.insert(3, "three");
      t.insert(4, "four");
      CHECK(t.size() == 4);
      CHECK(t.capacity() == 8);
      t.insert(5, "four");
      CHECK(t.capacity() == 16);
   }

   SECTION("make cluster") {
      hash_table<TestType> t(10);
      t.insert( 1, "one");
      t.insert(11, "two");
      t.insert(21, "three");
      t.insert(31, "four");
      t.insert(41, "five");
      CHECK(t.size() == 5);

      CHECK(t.search( 1));
      CHECK(t.search(11));
      CHECK(t.search(21));
      CHECK(t.search(31));
      CHECK(t.search(41));

      CHECK_FALSE(t.search(51));
   }

   SECTION("remove") {
      hash_table<TestType> t(10);
      t.insert( 3, "one");
      t.insert(13, "two");
      t.insert(23, "three");
      t.insert(33, "four");
      t.insert(43, "five");
      CHECK(t.size() == 5);

      CHECK(t.search(23));
      t.remove(23);
      CHECK_FALSE(t.search(23));

      CHECK(t.search(33));
      t.remove(33);
      CHECK_FALSE(t.search(33));

      CHECK(t.search(43));
      t.remove(43);
      CHECK_FALSE(t.search(43));

      CHECK(t.search(3));
      t.remove(3);
      CHECK_FALSE(t.search(3));

      CHECK(t.search(13));
      t.remove(13);
      CHECK_FALSE(t.search(13));

      CHECK(t.size() == 0);
   }

   SECTION("big table") {
      hash_table<TestType> t(10);

      constexpr auto sz = 10'000;
      for (size_t i = 0; i < sz; ++i) {
         t.insert(i, std::to_string(i));
      }

      for (size_t i = 0; i < sz; ++i) {
         CHECK(t.search(i));
      }

      for (size_t i = 0; i < sz; ++i) {
         t.remove(i);
      }

      for (size_t i = 0; i < sz; ++i) {
         CHECK_FALSE(t.search(i));
      }
   }
}
