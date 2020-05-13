#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <average.hpp>
#include <max.hpp>

// ------------------------------------------------------------------
TEST_CASE("max on range", "[max on range]") {
   range::max<4> rm;
   rm.add(1).add(2);
   CHECK(rm.get_value() == 2);
   rm.add(3).add(1);
   CHECK(rm.get_value() == 3);
   rm.add(4);
   CHECK(rm.get_value() == 4);
   rm.add(3).add(2).add(1);
   CHECK(rm.get_value() == 4);
   rm.add(1);
   CHECK(rm.get_value() == 3);
}

// ------------------------------------------------------------------
bool are_equal(float x, float y) {
   return std::abs(x - y) < 0.000'001;
}

// ------------------------------------------------------------------
TEST_CASE("average on range", "[average on range]") {
   range::average<4> ra;
   ra.add(1.);
   CHECK(ra.get_value() == 1.);  // 1
   ra.add(3.);
   CHECK(ra.get_value() == 2.);  // 1 3
   ra.add(4.).add(8.);
   CHECK(ra.get_value() == 4.);  // 1 3 4 8
   ra.add(1.);
   CHECK(ra.get_value() == 4.);  // 3 4 8 1
   ra.add(7.);
   CHECK(ra.get_value() == 5.);  // 4 8 1 7
   ra.add(8.);
   CHECK(ra.get_value() == 6.);  // 8 1 7 8
   ra.add(0.);
   CHECK(are_equal(ra.get_value(), 4.));  // 1 7 8 0
}