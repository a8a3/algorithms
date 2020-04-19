#pragma once

namespace utils {

template<typename T>
void swap(T* lhs, T* rhs) {
   auto tmp = *lhs;
   *lhs = *rhs;
   *rhs = tmp;
}

void swap(int& lhs, int& rhs) {
   auto tmp = lhs;
   lhs = rhs;
   rhs = tmp;
}

}  // namespace utils