#pragma once

#include <chrono>
#include <iostream>

// ------------------------------------------------------------------
class timer {
   using clock = std::chrono::steady_clock;
   const char* const what_;
   const clock::time_point start_{clock::now()};
public:
   explicit timer(const char* what) : what_(what) {}

   void check() const {
      std::cerr << what_ << " gets " << std::chrono::duration_cast<std::chrono::microseconds>(clock::now()-start_).count() << " us\n";
   }
};