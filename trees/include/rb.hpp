#pragma once

#include "bst.hpp"

namespace rb {

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

void check_1(node*);
void check_2(node*);
void check_3(node*);
void check_4(node*);
void check_5(node*);

// ------------------------------------------------------------------
void remove_helper(node* node) {
   if (node->left != LEAF) {
      remove_helper(node->left);
   }
   if (node->right != LEAF) {
      remove_helper(node->right);
   }
   delete node;
}

// ------------------------------------------------------------------
node* get_root(node* n) {
   if (!n) return nullptr;

   while(n->parent) {
      n = n->parent;
   }
   return n;
}

// ------------------------------------------------------------------
void remove(node* node) {
   remove_helper(get_root(node));
}

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
void check_5(node* n) {
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
         g->parent->left = p;
      }

      g->parent = p;
   } else {
      // rotate g to the left
      g->right = p->left;
      g->right->parent = g;

      p->left = g;
      p->parent = g->parent;

      if (g->parent) {
         g->parent->right = p;
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

// ------------------------------------------------------------------
node* insert(int value, node* root = nullptr) {
   node* inserted = nullptr;

   // TODO root may be changed by rotating of tree.
   //      keep root pointer in the class- wrapper.
   //      use PIMPL to realize class- wrapper.
   //      class- wrapper interface should give ability to test rb- tree via common tests for trees

   root = get_root(root);

   if (root) {
      node *current = root;
      while (!inserted) {
         if (value < current->val) {
            if (current->left == LEAF) {
               inserted = new node{current, LEAF, LEAF, value};
               current->left = inserted;
            }
            current = current->left;
         } else {
            if (current->right == LEAF) {
               inserted = new node{current, LEAF, LEAF, value};;
               current->right = inserted;
            }
            current = current->right;
         }
      }
   } else {
      inserted = new node{nullptr, LEAF, LEAF, value};
   }
   check_1(inserted);
   return inserted;
}

} // namespace rb