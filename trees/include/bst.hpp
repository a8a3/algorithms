#pragma once

#include <istream>

namespace bst {

// ------------------------------------------------------------------
class node {
public:
   explicit node(int val) : value_(val) {}
   ~node() {
      delete left_;
      left_ = nullptr;
      delete right_;
      right_ = nullptr;
   }

   node& insert(int val) {
      node* current = this;

      while (true) {
         if (val < current->value_) {
            if (current->left_) {
               current = current->left_;
            } else {
               current->left_ = new node(val);
               break; // while
            }
         } else {
            if (current->right_) {
               current = current->right_;
            } else {
               current->right_ = new node(val);
               break; // while
            }
         }
      }
      return *this;
   }

   int get_min_value() const {
      const node* current = this;

      while(current->left_) {
         current = current->left_;
      }
      return current->value_;
   }


// TODO
   node& remove(int val, node* parent = nullptr) {
      node* current = this;

      while(current) {
         if (val < current->value_) {
            parent = current;
            current = current->left_;
         } else if (val > current->value_) {
            parent = current;
            current = current->right_;
         } else {
            if (current->left_ && current->right_) {
#if 0
       10
       /
      8   <- remove
     / \
    7   9
#endif // 0
               current->value_ = current->right_->get_min_value();
               current->right_->remove(current->value_, current);
            } else if (!parent) {
#if 0
       10 <- remove                10    <- remove
       /                             \
      8                               16
     / \                              / \
    7   9                           12   22
#endif // 0
               if (current->left_) {
                  node* tmp = current->left_;
                  current->value_ = tmp->value_;
                  current->right_ = tmp->right_;
                  current->left_ = tmp->left_;

                  tmp->left_ = nullptr;
                  tmp->right_ = nullptr;
                  delete tmp;
               } else if (current->right_) {
                  node* tmp = current->right_;
                  current->value_ = tmp->value_;
                  current->left_ = tmp->left_;
                  current->right_ = tmp->right_;

                  tmp->left_ = nullptr;
                  tmp->right_ = nullptr;
                  delete tmp;
               }
            }
#if 0
        10                         10
       /                             \
      8  <- remove                    16  <- remove
     /                                  \
    7                                    22
#endif // 0
            else if (current == parent->left_) {
               parent->left_ = current->left_ ? current->left_ : current->right_;
               current->left_ = nullptr;
               current->right_ = nullptr;
               delete current;
            } else if (current == parent->right_) {
               parent->right_ = current->left_ ? current->left_ : current->right_;
               current->left_ = nullptr;
               current->right_ = nullptr;
               delete current;
            }
            break;
         }
      }
      return *this;
   }

   bool search(int val) const {
      const node* current = this;

      while (current) {
         if (val == current->value_) {
            return true;
         } else if (val < current->value_) {
            current = current->left_;
         } else {
            current = current->right_;
         }
      }
      return false;
   }

   int   get_value() const {return value_;}

   node* get_left () const {return left_;}
   node* get_right() const {return right_;}

   size_t size() const {
      size_t res = 1;
      if(left_)
         res += left_->size();

      if(right_)
         res += right_->size();
      return res;
   }

private:
   void set_left (node* n) {left_  = n;}
   void set_right(node* n) {right_ = n;}

   int value_;

   node* left_{nullptr};
   node* right_{nullptr};
};

// ------------------------------------------------------------------
bool is_search_tree_helper(const node* n, int min, int max) {
   if (!n) return true;

   const auto l = n->get_left();
   const auto r = n->get_right();

   bool res = true;
   if (l) {
      if (l->get_value() > max) return false;
      res = is_search_tree_helper(l, min, n->get_value());
   }

   if (res && r) {
      if (r->get_value() < min) return false;
      res = is_search_tree_helper(r, n->get_value(), max);
   }
   return res;
}

// ------------------------------------------------------------------
bool is_search_tree(node* n) {
   return is_search_tree_helper(n, INT32_MIN, INT32_MAX);
}

// ------------------------------------------------------------------
void print_helper(node* n) {
   if (!n) {
      return;
   }
   print_helper(n->get_left());
   std::clog << n->get_value() << ' ';
   print_helper(n->get_right());
}

// ------------------------------------------------------------------
void print(node* n) {
   std::clog << "bst: ";
   print_helper(n);
   std::clog << '\n';
}

} // namespace bst