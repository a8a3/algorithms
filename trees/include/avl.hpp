#pragma once

#include "bst.hpp"

namespace avl {

// ------------------------------------------------------------------
class node : public bst::node {
public:
   using bst::node::node;
   ~node() override = default;

   bst::node& insert(int val) override {
      bst::node::insert(val);
      rebalance();
      return *this;
   }

   bst::node& remove(int val, bst::node* parent = nullptr) override {
      bst::node::remove(val, parent);
      rebalance();
      return *this;
   }

   int get_height() const {return height_;}

   struct rotation_counter {
      size_t small_right{0};
      size_t small_left{0};
      size_t big_right{0};
      size_t big_left{0};
   };

   const rotation_counter& get_rotation_counters() const {
      return rc_;
   };

private:
   void small_right_rotation() {
      node * l = static_cast<node *>(left_);
      node * r = static_cast<node *>(right_);

      assert(l);

      std::swap(value_, l->value_);
      left_ = l->get_left();
      right_ = l;
      l->left_ = l->right_;
      l->right_ = r;

      static_cast<node*>(right_)->update_height();
      this->update_height();
   }

   void small_left_rotation() {
      node * l = static_cast<node *>(left_);
      node * r = static_cast<node *>(right_);

      assert(r);

      std::swap(value_, r->value_);
      left_ = r;
      right_ = r->get_right();
      r->right_ = r->left_;
      r->left_ = l;

      static_cast<node*>(left_)->update_height();
      this->update_height();
   }

   void big_left_rotation() {
      assert(right_);

      node* r = static_cast<node*>(right_);
      r->small_right_rotation();
      this->small_left_rotation();
   }

   void big_right_rotation() {
      assert(left_);

      node* l = static_cast<node*>(left_);
      l->small_left_rotation();
      this->small_right_rotation();
   }

   void update_height() {
      node* l = static_cast<node*>(left_);
      node* r = static_cast<node*>(right_);

      if (!l && !r) {
         height_ = 0;
      } else {
         int lh = l ? l->get_height() : 0;
         int rh = r ? r->get_height() : 0;
         height_ = std::max(lh, rh) + 1;
      }
   }

   void rebalance() {
      node* l = static_cast<node*>(left_);
      node* r = static_cast<node*>(right_);

      int hl = 0;

      if (l) {
         l->rebalance();
         hl = l->get_height();
      }

      int hr = 0;
      if (r) {
         r->rebalance();
         hr = r->get_height();
      }

      int balance = hl - hr;

      if (balance > 1) { // right rotation
         assert(l);

         int left_grand_h  = l->get_left()  ? static_cast<node*>(l->get_left())->get_height()  : 0;
         int right_grand_h = l->get_right() ? static_cast<node*>(l->get_right())->get_height() : 0;

         if (right_grand_h <= left_grand_h) {
            small_right_rotation();
            ++rc_.small_right;
         } else {
            big_right_rotation();
            ++rc_.big_right;
         }
      } else if (balance < -1) { // left rotation
         assert(r);

         int left_grand_h  = r->get_left()  ? static_cast<node*>(r->get_left())->get_height()  : 0;
         int right_grand_h = r->get_right() ? static_cast<node*>(r->get_right())->get_height() : 0;

         if (left_grand_h <= right_grand_h) {
            small_left_rotation();
            ++rc_.small_left;
         } else {
            big_left_rotation();
            ++rc_.big_left;
         }
      } else {
         update_height();
      }
   }

   node* create_node(int val) const override {
      return new node(val);
   }
   int height_{0};
   rotation_counter rc_;
};

} // namespace avl