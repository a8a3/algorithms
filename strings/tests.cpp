#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>
#include <vector>

#include <boyer_moor.hpp>
#include <fsm.hpp>
#include <kmp.hpp>

// ------------------------------------------------------------------
template<int F(const std::string&, const std::string&)>
struct search {
   int operator() (const std::string& text, const std::string& pattern) {
      return F(text, pattern);
   }
};

// ------------------------------------------------------------------
TEMPLATE_TEST_CASE("substring_search", "[strings][template]",
   search<strings::simple_search>,
   search<strings::boyer_moor_horspool>,
   search<strings::boyer_moor>,
   search<kmp::search>) {

   struct data {
      const std::string text;
      const std::string pattern;
      const int pos{-1};
   };

   std::vector<data> d {
      {"OKOMOKOLOKOL",   "KOLOKOL",  5},
      {"SOMESTRING",      "STRING",  4},
      {"SOMESTRING",        "SOME",  0},
      {"SOMESTRING",         "ING",  7},
      {"SOMESTRING",     "NOTHING", -1},
      {"SOMESTRING",  "SOMESTRING",  0},
      {"SOMESTRING", "EXTRASTRING", -1},
      {"SOMES*RINGST*INGSTR*NGSTRI*GSTRIN*STRING", "STRING",34},
      {"KOLOROLROLOKOLOKOMOKOLOKOL","KOLOKOL",  19}
   };

   SECTION("search") {
      std::for_each(d.begin(), d.end(),
      [](const auto& item){
         CHECK(TestType()(item.text, item.pattern) == item.pos);
      });

   }
}

// ------------------------------------------------------------------
TEST_CASE("suffix_build_test", "[boyer_moor]") {

   struct data {
      std::string prefix;
      std::vector<int> tbl;
   };

   std::vector<data> t{
      { "abcdadcd", {2, 4, 8, 8, 8, 8, 8, 8, 1}},
      { "abbad"   , {5, 5, 5, 5, 5, 1}},
      { "kolokol" , {4, 4, 4, 4, 4, 4, 4, 1}},
      { "string"  , {6, 6, 6, 6, 6, 6, 1}}
   };

   std::for_each(t.begin(), t.end(),
   [](const auto& d){
      CHECK(strings::build_suffix(d.prefix) == d.tbl);
   });
}

// ------------------------------------------------------------------
TEST_CASE("fsm_tests", "[kmp]") {
   CHECK(fsm::left("string", 0).empty());
   CHECK(fsm::left("string", 3) == "str");
   CHECK(fsm::left("string", 6) == "string");

   CHECK(fsm::right("string", 0).empty());
   CHECK(fsm::right("string", 3) == "ing");
   CHECK(fsm::right("string", 6) == "string");

   fsm::table t = fsm::build_table("ABAB");
   CHECK(t == fsm::table{{1, 0},
                         {1, 2},
                         {3, 0},
                         {1, 4}});

   CHECK(fsm::search("ABAAABAB", "ABAB") == 4);
   CHECK(fsm::search("ABAAABAB",  "ABA") == 0);
   CHECK(fsm::search("ABAAABAB",  "AAA") == 2);
}

// ------------------------------------------------------------------
TEST_CASE("kmp_tests", "[kmp]") {
   kmp::pi_table t_slow = kmp::build_pi_table_slow("AABAAABAAAAB");
   CHECK(t_slow == kmp::pi_table{0,1,0,1,2,2,3,4,5,6,2,3});

   kmp::pi_table t_fast = kmp::build_pi_table_slow("AABAAABAAAAB");
   CHECK(t_slow == t_fast);
}