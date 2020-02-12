#pragma once

#include <array>

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
   std::array<uint64_t, 4> m_{1, 1,
                              1, 0};

   matrix& operator*=(const matrix& rhs) {
      // 0, 1 * 0, 1 = 00 + 12, 01 + 13
      // 2, 3   2, 3   20 + 32, 21 + 33
      matrix m(*this);

      m.m_[0] = m_[0]*rhs.m_[0] + m_[1]*rhs.m_[2];
      m.m_[1] = m_[0]*rhs.m_[1] + m_[1]*rhs.m_[3];

      m.m_[2] = m_[2]*rhs.m_[0] + m_[3]*rhs.m_[2];
      m.m_[3] = m_[2]*rhs.m_[1] + m_[3]*rhs.m_[3];

      m_.swap(m.m_);
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
   uint64_t get_value(uint64_t num) const {
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

