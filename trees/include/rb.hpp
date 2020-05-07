#pragma once

#include <memory>

class tree_impl;

namespace rb {
class tree {
public:
   tree();
   ~tree();

   tree& insert(int val);
   bool search(int val);

private:
   std::unique_ptr<tree_impl> impl_;
};
} // namespace rb