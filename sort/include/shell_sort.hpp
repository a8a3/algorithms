#pragma once

#include <algorithm>
#include <iterator>

namespace shell {
// ------------------------------------------------------------------
void swap(int *lhs, int *rhs) {
   int tmp = *lhs;
   *lhs = *rhs;
   *rhs = tmp;
}
}

// ------------------------------------------------------------------
template<typename T>
bool equal(const T& lhs, const T& rhs) {
   return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs));
}

// ------------------------------------------------------------------
namespace gap {

class shell {
public:
   explicit shell(size_t sz) : gap_(sz) {}

   size_t next_gap() {
      gap_ >>= 1;
      return gap_;
   }

private:
   size_t gap_;
};


class hibbard {
public:
   explicit hibbard(size_t sz) : p_(sz > 0 ? std::log2(sz) : 0) {}

   size_t next_gap() {
      if (p_ > 0) {
         --p_;
      }
      return std::pow(2., p_) - 1;
   }
private:
   size_t p_;
};


class sedgewick {
public:
   explicit sedgewick(size_t sz) : sz_(sz), p_ (sz > 0 ? std::log2(sz) : 0) {}

   size_t next_gap() {
      if (p_ > 1) {
         --p_;
      } else if (p_ == 1) {
         --p_;
         return 1;
      } else {
         return 0;
      }
      auto res = std::pow(4., p_) + 3*std::pow(2., p_-1) + 1;
      while (res > sz_) {
         res = next_gap();
      }
      return res;
   }
private:
   const size_t sz_;
   size_t p_;
};
} // namespace gap

// ------------------------------------------------------------------
template<typename G = gap::shell>
void shell_sort(int* arr, size_t sz) {
   G g(sz);
   size_t gap = g.next_gap();

   while (gap > 0) {
      int* fwd = arr + gap;

      while (fwd < arr + sz) {
         int* r = fwd;

         while (r > arr && *r < *(r-gap)) {
            shell::swap(r, r-gap);
            r -= gap;
         }
         fwd += gap;
      }
      gap = g.next_gap();
   }
}
