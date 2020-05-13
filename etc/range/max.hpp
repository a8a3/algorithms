#pragma once

#include <queue>

namespace range {

// ------------------------------------------------------------------
template<size_t R>
class max {
public:
   max() = default;

   max& add(int value) {
      if (dq_.size() == R) {
         dq_.pop_front();
      }
      while (!dq_.empty() && value >= dq_.back()) {
         dq_.pop_back();
      }
      dq_.push_back(value);
      return *this;
   }

   int get_value() const {
      return dq_.front();
   }

private:
   std::deque<int> dq_;
};

} // range