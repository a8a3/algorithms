
#include <cassert>
#include <cstdint>
#include <iostream>

#include <rb.hpp>

// ------------------------------------------------------------------
enum class color : uint8_t {
   black = 0,
   red   = 1
};

// ------------------------------------------------------------------
struct node {
   node* parent{nullptr};
   node* left{nullptr};
   node* right{nullptr};

   int val{0};
   color clr{color::red};
};

node leaf{nullptr, nullptr, nullptr, INT32_MIN, color::black};
node* LEAF = &leaf;

// ------------------------------------------------------------------
node* get_grandparent(node* n) {
   assert(n && n->parent);
   return n->parent->parent;
}

// ------------------------------------------------------------------
node* get_uncle(node* n) {
   node* grand_parent = get_grandparent(n);
   assert(grand_parent);

   if (n->parent == grand_parent->left) {
      return grand_parent->right;
   }
   return grand_parent->left;
}

// ------------------------------------------------------------------
class tree_impl {
public:

~tree_impl() {
   delete_nodes(root_);
}

void insert(int val) {
   node* inserted = nullptr;

   if (root_) {
      node* current = root_;

      while (true) {
         if (val < current->val) {
            if (current->left == LEAF) {
               inserted = current->left = new node{current, LEAF, LEAF, val};
               break; // while
            }
            current = current->left;
         } else {
            if (current->right == LEAF) {
               inserted = current->right = new node{current, LEAF, LEAF, val};
               break; // while
            }
            current = current->right;
         }
      }
   } else {
      inserted = root_ = new node{nullptr, LEAF, LEAF, val};
   }
   check_1(inserted);
}

// ------------------------------------------------------------------
bool search(int val) {
   node* current = root_;

   while(current != LEAF) {
      if (val == current->val) {
         return true;
      } else if (val < current->val) {
         current = current->left;
      } else {
         current = current->right;
      }
   }
   return false;
}

private:

// ------------------------------------------------------------------
void delete_nodes(node* n) {
   if (!n || n == LEAF) return;

   if (n->left != LEAF) {
      delete_nodes(n->left);
   }
   if (n->right != LEAF) {
      delete_nodes(n->right);
   }
   delete n;
}

// ------------------------------------------------------------------
void check_5(node* n) {
// std::clog << __PRETTY_FUNCTION__ << '\n';
   assert(n);

   node* p = n->parent;
   node* g = get_grandparent(n);
   assert(g);

   assert(n->parent->clr == color::red);
   n->parent->clr = color::black;

   assert(g->clr == color::black);
   g->clr = color::red;

   if (n == n->parent->left && n->parent == g->left) {
      // rotate g to the right
      g->left = p->right;
      g->left->parent = g;

      p->right = g;
      p->parent = g->parent;

      if (g->parent) {
         if (g == g->parent->left) {
            g->parent->left = p;
         } else {
            g->parent->right = p;
         }
      } else {
         root_ = p;  // root was rotated to the right, p is new root now
      }

      g->parent = p;
   } else {
      // rotate g to the left
      g->right = p->left;
      g->right->parent = g;

      p->left = g;
      p->parent = g->parent;

      if (g->parent) {
         if (g == g->parent->left) {
            g->parent->left = p;
         } else {
            g->parent->right = p;
         }
      } else {
         root_ = p; // root was rotated to the left, p is new root now
      }

      g->parent = p;
   }
}

// ------------------------------------------------------------------
void check_4(node* n) {
   assert(n);

   node* g = get_grandparent(n);
   assert(g);

   // if n and parent are not in line, put them in one line with rotation
   if (n == n->parent->right && n->parent == g->left) {
      // rotate parent to the left
      node* p = n->parent;
      g->left = n;
      p->right = n->left;
      n->left = p;

      n->parent = g;
      p->parent = n;
      p->right->parent = p;

      n = n->left;
   } else if (n == n->parent->left && n->parent == g->right) {
      // rotate parent to the right
      node* p = n->parent;
      g->right = n;
      p->left = n->right;
      n->right = p;

      n->parent = g;
      p->parent = n;
      p->left->parent = p;

      n = n->right;
   }
   check_5(n);
}

// ------------------------------------------------------------------
void check_3(node* n) {
   assert(n && n->parent);

   node* u = get_uncle(n);
   assert(u);

   // check uncle color
   if (u->clr == color::red) {
      assert(n->parent->clr == color::red);

      // recolor parent, uncle in black, grandparent in red
      n->parent->clr = color::black;
      u->clr = color::black;

      node* g = get_grandparent(n);
      assert(g);

      g->clr = color::red;

      check_1(g); // check rb-tree conditions for grandparent
   } else {
      check_4(n);
   }
}

// ------------------------------------------------------------------
void check_2(node* n) {
   assert(n && n->parent);

   if (n->parent->clr == color::red) {
      check_3(n);
   }
   // otherwise tree is correct
}

// ------------------------------------------------------------------
void check_1(node* n) {
   assert(n);

   if (n->parent == nullptr) {
      n->clr = color::black;
   } else {
      check_2(n);
   }
}

node* root_{nullptr};
}; // tree_impl


namespace rb {

// ------------------------------------------------------------------
tree::tree() : impl_( new tree_impl() )
{}

// ------------------------------------------------------------------
tree::~tree() = default;

// ------------------------------------------------------------------
tree& tree::insert(int val) {
   impl_->insert(val);
   return *this;
}

// ------------------------------------------------------------------
bool tree::search(int val) {
   return impl_->search(val);
}

} // namespace rb