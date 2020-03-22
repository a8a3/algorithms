#define CATCH_CONFIG_MAIN   // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <iostream>
#include <vector>

using permutations = std::vector<std::vector<int>>;

// ------------------------------------------------------------------
std::vector<int> start_with(size_t idx, const std::vector<int>& v) {
   std::vector<int> result{v.begin(), v.end()};
   std::swap(result[0], result[idx]);
   return result;
}

const auto print = [] (const auto& about, const auto& what) {
   std::cerr << about << '\n';
   for (const auto& i: what) {
      std::cerr << i << ' ';
   }
   std::cerr << '\n';
};

// ------------------------------------------------------------------
permutations get_permutations(const std::vector<int>& origin) {
   permutations ps;

   if (origin.size() == 1) {
      ps.emplace_back(origin);
      return ps;
   }

   for (size_t i = 0, e = origin.size(); i < e; ++i) {
      const auto this_gen = start_with(i, origin);

      if (this_gen.size() < 3) {
         ps.emplace_back(this_gen);
      } else {
         const auto next_gen = get_permutations({std::next(this_gen.begin()), this_gen.end()});
         for (const auto &ng : next_gen) {
            std::vector<int> mut;
            mut.reserve(this_gen.size());
            mut.push_back(this_gen.front());
            mut.insert(mut.end(), ng.begin(), ng.end());
            ps.emplace_back(std::move(mut));
         }
      }
   }
   return ps;
}

// ------------------------------------------------------------------
TEST_CASE("test", "[permutations]") {

   std::vector<int> origin{1, 2, 3};
   const auto ps = get_permutations(origin);

   for (const auto& p : ps) {
      print("mutation: ", p);
   }
}