#include <benchmark/benchmark.h>

#include "include/dynamic_array.hpp"

namespace {
constexpr size_t FACTOR{32};
} // namespace

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
BENCHMARK_TEMPLATE(BM_back_insertion, single_array<int>        )->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_back_insertion, vector_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_back_insertion, factor_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_back_insertion, matrix_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);

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
BENCHMARK_TEMPLATE(BM_back_deletion, single_array<int>        )->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_back_deletion, vector_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_back_deletion, factor_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_back_deletion, matrix_array<int, FACTOR>)->Iterations(128)->RangeMultiplier(10)->Range(10, 1'000);

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
BENCHMARK_TEMPLATE(BM_front_insertion, single_array<int>)->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_front_insertion, vector_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_front_insertion, factor_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_front_insertion, matrix_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);

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
BENCHMARK_TEMPLATE(BM_front_deletion, single_array<int>        )->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_front_deletion, vector_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_front_deletion, factor_array<int, FACTOR>)->RangeMultiplier(10)->Range(10, 1'000);
BENCHMARK_TEMPLATE(BM_front_deletion, matrix_array<int, FACTOR>)->Iterations(128)->RangeMultiplier(10)->Range(10, 1'000);

// TODO
// ------------------------------------------------------------------
//class dynamic_array_fixture : public benchmark::Fixture {
//public:
//   single_array<int> array_;
//
//   void SetUp(const ::benchmark::State& state) {
//
//      for(size_t i = 0, sz = state.range(0); i < sz; ++i) {
//         array_.add_back(42);
//      }
//   }
//   void TearDown(const ::benchmark::State& ) {}
//};
//
//BENCHMARK_DEFINE_F(dynamic_array_fixture, back_deletion)(benchmark::State& st) {
//   for (auto _ : st) {
//      array_.remove(0);
//   }
//}
//BENCHMARK_REGISTER_F(dynamic_array_fixture, back_deletion)->RangeMultiplier(10)->Range(10, 1'000);

BENCHMARK_MAIN();