#pragma once

#include <array>

#include <array>
#include <math.h>


// ------------------------------------------------------------------
// time O(2^n), space O(2^n)
uint32_t recursive_fib(uint32_t num) {
    if (num < 2) return num;
    return recursive_fib(num - 1) + recursive_fib(num - 2);
}

// ------------------------------------------------------------------
// time O(n), space O(1)
uint32_t cicle_fib(uint32_t num) {
    if (num < 2) return num;

    uint32_t current{ 1 };
    uint32_t next{ 1 };

    while (--num > 0) {
        next = next + current;
        current = next - current;
    }

    return current;
}

// ------------------------------------------------------------------
// time O(1), space O(1)
uint32_t golden_ratio_fib(uint8_t num) {
    if (num < 2) return num;

    const double sq = std::sqrt(5);
    const double phi = (1 + sq) / 2;
    return static_cast<uint32_t>(std::pow(phi, num) / sq + .5);
}

// ------------------------------------------------------------------
template <uint32_t>
struct fib;

template<>
struct fib<0> {
   static constexpr uint32_t value{0};
};

template<>
struct fib<1> {
   static constexpr uint32_t value{1};
};

template<uint32_t Tn>
struct fib {
   static constexpr uint32_t value = fib<Tn-1>::value + fib<Tn-2>::value;
};

// ------------------------------------------------------------------
struct matrix {
#if 0
   std::array<uint64_t, 4> m_{1, 1,
                              1, 0};
#else
   uint64_t m_[4] = {1, 1, 1, 0};
#endif // 0
   constexpr matrix& operator*=(const matrix& rhs) {
      // 0, 1 * 0, 1 = 00 + 12, 01 + 13
      // 2, 3   2, 3   20 + 32, 21 + 33
      matrix m(*this);

      m.m_[0] = m_[0]*rhs.m_[0] + m_[1]*rhs.m_[2];
      m.m_[1] = m_[0]*rhs.m_[1] + m_[1]*rhs.m_[3];

      m.m_[2] = m_[2]*rhs.m_[0] + m_[3]*rhs.m_[2];
      m.m_[3] = m_[2]*rhs.m_[1] + m_[3]*rhs.m_[3];

      for (size_t i = 0, sz = sizeof(m_)/sizeof(m_[0]); i < sz; ++i) {
         *(m_ + i) = *(m.m_ + i);
      }
      return *this;
   }
};

std::ostream& operator <<(std::ostream& os, const matrix& m) {
   os << "matrix:\n"
      << m.m_[0] << ' ' << m.m_[1] << '\n'
      << m.m_[2] << ' ' << m.m_[3] << '\n';
   return os;
}

// ------------------------------------------------------------------
uint64_t pow_of2_pow(uint64_t base, uint64_t p) {
   uint64_t result{1};

   while (p > 0) {
      if (p & 1) {
         result *= base;
      }
      base *= base;
      p >>= 1;
   }
   return result;
}

// ------------------------------------------------------------------
struct fib_matrix {
   constexpr static uint64_t get_value(uint64_t num) {
      if (num < 2) return num;

      matrix result;
      matrix transform_matrix;
      num -= 2;  // result matrix already contains up to second fib numbers

      while(num > 0) {
         if (num & 1) {
            result *= transform_matrix;
         }
         transform_matrix *= transform_matrix;
         num >>= 1;
      };
      return result.m_[0];
   }
};

