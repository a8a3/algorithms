#pragma once

#include <algorithm>
#include <iterator>
#include <random>

// ------------------------------------------------------------------
template<typename T>
void make_shuffle(T* what, size_t sz) {
   std::random_device randomDevice;
   std::mt19937 generator(randomDevice());
   std::shuffle(what, what + sz, generator);
}

// ------------------------------------------------------------------
constexpr size_t get_percent(size_t total, size_t percent) {
   return static_cast<size_t>(total * (percent/100.));
}
