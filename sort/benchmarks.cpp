#include <benchmark/benchmark.h>

#if 0
#include <memory>
#include <numeric>
#include <vector>
#include <shuffle.hpp>
#include "include/shell_sort.hpp"
#include "include/heap_sort.hpp"

namespace settings {
constexpr auto SIZE = 100'000;
constexpr auto UNSORTED = get_percent(SIZE, 5);
} // namespace settings


// ------------------------------------------------------------------
template<typename G>
struct shell_sort_call {
   void operator () (int* arr, size_t sz) {
      shell_sort<G>(arr, sz);
   }
};

// ------------------------------------------------------------------
struct heap_sort_call {
   void operator () (int* arr, size_t sz) {
      heap_sort::sort(arr, sz);
   }
};

// ------------------------------------------------------------------
template<typename SortPolicy>
static void BM_shuffled_array_sort(benchmark::State& state) {
   for (auto _ : state) {
      state.PauseTiming();
      std::unique_ptr<int[]> arr(new int[settings::SIZE]);
      std::iota(arr.get(), arr.get() + settings::SIZE, 0);
      make_shuffle(arr.get(), settings::SIZE);
      state.ResumeTiming();

      SortPolicy()(arr.get(), settings::SIZE);
   }
}
BENCHMARK_TEMPLATE(BM_shuffled_array_sort, shell_sort_call<gap::shell>)->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_shuffled_array_sort, shell_sort_call<gap::hibbard>)->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_shuffled_array_sort, shell_sort_call<gap::sedgewick>)->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_shuffled_array_sort, heap_sort_call)->Unit(benchmark::kMillisecond);


// ------------------------------------------------------------------
template<typename SortPolicy>
static void BM_near_sorted_array_sort(benchmark::State& state) {
   for (auto _ : state) {
      state.PauseTiming();
      std::unique_ptr<int[]> arr(new int[settings::SIZE]);
      std::iota(arr.get(), arr.get() + settings::SIZE, 0);
      make_shuffle(arr.get(), settings::UNSORTED);
      state.ResumeTiming();

      SortPolicy()(arr.get(), settings::SIZE);
   }
}
BENCHMARK_TEMPLATE(BM_near_sorted_array_sort, shell_sort_call<gap::shell>)->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_near_sorted_array_sort, shell_sort_call<gap::hibbard>)->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_near_sorted_array_sort, shell_sort_call<gap::sedgewick>)->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_near_sorted_array_sort, heap_sort_call)->Unit(benchmark::kMillisecond);

#else // 0

#include <algorithm>
#include <numeric>

#include <external_sort.hpp>

static void BM_external_sort(benchmark::State& state) {
   for (auto _ : state) {
      state.PauseTiming();
      constexpr auto shuffled_file_name = "shuffled.bin";
      const int64_t default_random_range = 65'535;
      create_shuffled_binary_file(shuffled_file_name, state.range(0), std::min(state.range(0), default_random_range));
      state.ResumeTiming();

      sort_file(shuffled_file_name, state.range(1));
   }
}
BENCHMARK(BM_external_sort)->Unit(benchmark::kMillisecond)->Args({1'048'576,  8})
                                                          ->Args({1'048'576, 16})
                                                          ->Args({1'048'576, 32})
                                                          ->Args({1'048'576, 64});
// 1'073'741'824

#endif // 0

BENCHMARK_MAIN();