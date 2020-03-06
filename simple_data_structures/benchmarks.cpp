#include <benchmark/benchmark.h>

#include "include/dynamic_array.hpp"

namespace settings {
constexpr size_t factor{32};
constexpr size_t iterations{128};
struct range {
   constexpr static size_t start{10};
   constexpr static size_t end  {100};
   constexpr static size_t mul  {10};
};
} // settings

using namespace settings;

// ------------------------------------------------------------------
template <class TA>
void BM_back_insertion(benchmark::State& state) {
   TA array;

   for (auto _ : state) {
      for(size_t i = 0, sz = state.range(0); i < sz; ++i) {
         array.add_back(42);
      }
   }
}
BENCHMARK_TEMPLATE(BM_back_insertion, single_array<int>        )->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_back_insertion, vector_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_back_insertion, factor_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_back_insertion, matrix_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);

// ------------------------------------------------------------------
template <class TA>
void BM_back_deletion(benchmark::State& state) {
   for (auto _ : state) {
      state.PauseTiming();
      TA array;
      const auto array_size = state.range(0);

      for(long i = 0; i < array_size; ++i) {
         array.add_back(42);
      }
      state.ResumeTiming();

      for(long i = 0; i < array_size; ++i) {
         array.remove(array.size()-1);
      }
   }
}
BENCHMARK_TEMPLATE(BM_back_deletion, single_array<int>        )->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_back_deletion, vector_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_back_deletion, factor_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_back_deletion, matrix_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);

// ------------------------------------------------------------------
template <class TA>
void BM_front_insertion(benchmark::State& state) {
   TA array;

   for (auto _ : state) {
      for(size_t i = 0, sz = state.range(0); i < sz; ++i) {
         array.add_front(42);
      }
   }
}
BENCHMARK_TEMPLATE(BM_front_insertion, single_array<int>        )->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_front_insertion, vector_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_front_insertion, factor_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_front_insertion, matrix_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);

// ------------------------------------------------------------------
template <class TA>
void BM_front_deletion(benchmark::State& state) {
   for (auto _ : state) {
      state.PauseTiming();
      TA array;
      const auto array_size = state.range(0);

      for(long i = 0; i < array_size; ++i) {
         array.add_back(42);
      }
      state.ResumeTiming();

      for(long i = 0; i < array_size; ++i) {
         array.remove(0);
      }
   }
}
BENCHMARK_TEMPLATE(BM_front_deletion, single_array<int>        )->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_front_deletion, vector_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_front_deletion, factor_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_front_deletion, matrix_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);

// ------------------------------------------------------------------
template <class TA>
void BM_get_by_index(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        TA array;
        const auto array_size = state.range(0);

        for (long i = 0; i < array_size; ++i) {
            array.add_back(42);
        }
        state.ResumeTiming();

        for (long i = 0; i < array_size; ++i) {
            array.get(i);
        }
    }
}
BENCHMARK_TEMPLATE(BM_get_by_index, single_array<int>        )->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_get_by_index, vector_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_get_by_index, factor_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);
BENCHMARK_TEMPLATE(BM_get_by_index, matrix_array<int, factor>)->Iterations(iterations)->RangeMultiplier(range::mul)->Range(range::start, range::end);

BENCHMARK_MAIN();