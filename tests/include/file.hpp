#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <stack>
#include <vector>

#include <merge_sort.hpp>
#include "shuffle.hpp"

namespace file {

class chunk_t {
   int gen{0};

public:
   int  get_gen() const {return gen;}
   void inc_gen()       {++gen;}
};

using chunks_t = std::vector<chunk_t>;
std::stack<chunk_t> stack;

void print(std::stack<chunk_t> s) {
   while(!s.empty()) {
      const auto& top = s.top();
      std::clog << top.get_gen() << '\n';
      s.pop();
   }
   std::clog << "---\n";
}

// ------------------------------------------------------------------
void apply_chunk(const chunk_t& c) {

   if (stack.empty() || stack.top().get_gen() != c.get_gen()) {
      stack.push(c);
      return;
   }
   auto& top = stack.top();
   stack.pop();
   top.inc_gen();

   apply_chunk(top);
}

} // namespace file


