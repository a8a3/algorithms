#pragma once

#include <iostream>
#include <istream>
#include <queue>

namespace bst {

// ------------------------------------------------------------------
class node {
public:
   explicit node(int val) : value_(val) {}

   node(const node&) = delete;
   node&operator =(const node&) = delete;

   virtual ~node() {
      delete left_;
      left_ = nullptr;
      delete right_;
      right_ = nullptr;
   }

   virtual node& insert(int val) {
      node* current = this;

      while (true) {
         if (val < current->value_) {
            if (current->left_) {
               current = current->left_;
            } else {
               current->left_ = create_node(val);
               break; // while
            }
         } else {
            if (current->right_) {
               current = current->right_;
            } else {
               current->right_ = create_node(val);
               break; // while
            }
         }
      }
      return *this;
   }

   virtual node& remove(int val, node* parent = nullptr) {
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

   int   get_value () const {return value_;}
   node* get_left  () const {return left_;}
   node* get_right () const {return right_;}

   size_t size() const {
      size_t res = 1;
      if(left_)
         res += left_->size();

      if(right_)
         res += right_->size();
      return res;
   }

protected:
   int   value_;
   node* left_ {nullptr};
   node* right_{nullptr};

private:
   int get_min_value() const {
      const node* current = this;

      while(current->left_) {
         current = current->left_;
      }
      return current->value_;
   }

   virtual node* create_node(int val) const {
      return new node(val);
   }
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

// ------------------------------------------------------------------
struct storage {
   node* n{nullptr};
   node* p{nullptr};
   int level{0};
};

// ------------------------------------------------------------------
void print_by_level(node* n) {
   std::queue<storage> q;
   int level{0};
   q.push({n, nullptr, level});

   while(!q.empty()) {
      while (!q.empty() && q.front().level == level) {
         const storage& s = q.front();
         const std::string parent_val(s.p ? std::to_string(s.p->get_value()) : "r");
         std::clog << s.n->get_value() << '(' << parent_val << ')' << ' ';

         node* l = s.n->get_left();
         node* r = s.n->get_right();

         if (l)
            q.push({l, s.n, level+1});
         if (r)
            q.push({r, s.n, level+1});

         q.pop();
      }
      std::clog << '\n';
      ++level;
   }
}

} // namespace bst