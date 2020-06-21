#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file

#include <catch.hpp>
#include <vector>

#include <boyer_moor.hpp>

// ------------------------------------------------------------------
template<int F(const std::string&, const std::string&)>
struct search {
   int operator() (const std::string& text, const std::string& pattern) {
      return F(text, pattern);
   }
};

// ------------------------------------------------------------------
TEMPLATE_TEST_CASE("boyer_moor_substring_search", "[strings][template]",
   search<strings::simple_search>,
   search<strings::boyer_moor_horspool>,
   search<strings::boyer_moor>) {

   struct data {
      const std::string text;
      const std::string pattern;
      const int pos{-1};
   };

   std::vector<data> d {

//      {"OKOMOKOLOKOL",   "KOLOKOL",  5},
//      {"SOMESTRING",      "STRING",  4},
//      {"SOMESTRING",        "SOME",  0},
//      {"SOMESTRING",         "ING",  7},
//      {"SOMESTRING",     "NOTHING", -1},
//      {"SOMESTRING",  "SOMESTRING",  0},
//      {"SOMESTRING", "EXTRASTRING", -1},

      {"SOMES*RINGST*INGSTR*NGSTRI*GSTRIN*STRING", "STRING",34}
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
//         {"abcdadcd", {2, 4, 8, 8, 8, 8, 8, 8, 1}},
//         {   "abbad", {5, 5, 5, 5, 5, 1}},
//         { "kolokol", {4, 4, 4, 4, 4, 4, 4, 1}}
   };

   std::for_each(t.begin(), t.end(),
   [](const auto& d){
      CHECK(strings::build_suffix(d.prefix) == d.tbl);
   });
}