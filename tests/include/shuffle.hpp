#pragma once

#include <algorithm>
#include <iterator>
#include <random>

// ------------------------------------------------------------------
template<typename T>
void make_shuffle(T& what) {
   std::random_device randomDevice;
   std::mt19937 generator(randomDevice());
   std::shuffle(std::begin(what), std::end(what), generator);
}


