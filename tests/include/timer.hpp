#pragma once

#include <chrono>
#include <iostream>

// ------------------------------------------------------------------
class timer {
   using clock = std::chrono::steady_clock;
   const clock::time_point start_{clock::now()};
public:
    ~timer() {std::cerr << "elapsed time: " << std::chrono::duration_cast<std::chrono::microseconds>(clock::now()-start_).count() << " us\n";}

};