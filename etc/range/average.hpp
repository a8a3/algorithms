#pragma once

#include <queue>

namespace range {

// ------------------------------------------------------------------
template<size_t R, std::enable_if_t<(R>1), void*> = nullptr>
class average {
public:
   average() = default;

#if 0
   (10+20)/2 = 15
   (10+20)/3 = 15 - 15/3 = 10
   (10+20)/3 = 10 + 10/2 = 15

   avg(n+1) = avg(n) - avg(n)/(n+1)
   avg(n-1) = avg(n) + avg(n)/(n-1)

   q is full
   avg(n-1) - x/(n-1)

   other cases
   avg(n+1) + x/(n+1)
#endif

   average& add(float value) {
      if (q_.size() == R) {
         // pop oldest element
         const float front = q_.front();
         q_.pop();

         assert(q_.size() > 1);
         avg_ = avg_ + avg_/q_.size() - front/q_.size();
      }
      // push new element
      q_.push(value);
      avg_ = avg_ - avg_/q_.size() + value/q_.size();
      return *this;
   }

   float get_value() const {
      return avg_;
   }


private:
   std::queue<float> q_;
   float             avg_{0.};
};

} // namespace range