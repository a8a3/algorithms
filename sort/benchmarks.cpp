#include <benchmark/benchmark.h>

#include <memory>
#include <numeric>
#include <vector>

#include <shuffle.hpp>

#include <shell_sort.hpp>
#include <heap_sort.hpp>

#include <external_sort/bucket_sort.hpp>
#include <external_sort/quick_sort.hpp>

namespace settings {
constexpr auto SIZE = 100'000;
constexpr auto UNSORTED = get_percent(SIZE, 5);
} // namespace settings
-----------------------------------------------------------------
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


// ------------------------------------------------------------------
static void BM_external_quick_sort(benchmark::State& state) {
   for (auto _ : state) {
      state.PauseTiming();
      constexpr auto shuffled_file_name = "shuffled.bin";
      const int64_t default_random_range = 65'535;
      external_sort::create_shuffled_binary_file(shuffled_file_name, state.range(0), std::min(state.range(0), default_random_range));
      state.ResumeTiming();

      external_sort::quick::sort_file(shuffled_file_name, state.range(1));
   }
}
BENCHMARK(BM_external_quick_sort)->Unit(benchmark::kMillisecond)->Args({    1'048'576,  16})->
                                                                  Args({    1'048'576,  32})->
                                                                  Args({    1'048'576,  64})->
                                                                  Args({    1'048'576, 128})->
                                                                  Args({1'073'741'824, 128});


// ------------------------------------------------------------------
static void BM_external_bucket_sort(benchmark::State& state) {
   for (auto _ : state) {
      state.PauseTiming();
      constexpr auto shuffled_file_name = "shuffled.bin";
      const int64_t max_val = 65'000;
      external_sort::create_shuffled_binary_file(shuffled_file_name, state.range(0), max_val);
      state.ResumeTiming();

      external_sort::bucket::sort_file(shuffled_file_name, state.range(1), state.range(2), max_val);
   }
}
BENCHMARK(BM_external_bucket_sort)->Unit(benchmark::kMillisecond)->Args({    1'000'000,   10, 650})
                                                                 ->Args({    1'000'000,  100, 650})
                                                                 ->Args({    1'000'000, 1000, 650})
                                                                 ->Args({1'000'000'000,  100, 650});

//--------------------------------------------------------------------------
//Benchmark                                Time             CPU   Iterations
//--------------------------------------------------------------------------
//BM_external_sort/1048576/16           3648 ms         3579 ms            1
//BM_external_sort/1048576/32           1915 ms         1823 ms            1
//BM_external_sort/1048576/64           1124 ms         1032 ms            1
//BM_external_sort/1048576/128           577 ms          551 ms            1
//BM_external_sort/1073741824/128     627501 ms       609792 ms            1

//-------------------------------------------------------------------------------------
//Benchmark                                           Time             CPU   Iterations
//-------------------------------------------------------------------------------------
//BM_external_bucket_sort/1000000/10/650           6510 ms         6309 ms            1
//BM_external_bucket_sort/1000000/100/650          1088 ms         1060 ms            1
//BM_external_bucket_sort/1000000/1000/650        14947 ms        14899 ms            1
//BM_external_bucket_sort/1000000000/100/650    1195931 ms      1156432 ms            1

//--------------------------------------------------------------------------------------
//Benchmark                                            Time             CPU   Iterations
//--------------------------------------------------------------------------------------
//BM_external_bucket_sort/1000000/10/650            7431 ms         6941 ms            1
//BM_external_bucket_sort/1000000/100/650           1355 ms         1323 ms            1
//BM_external_bucket_sort/1000000/1000/650         20926 ms        20646 ms            1
//BM_external_bucket_sort/1000000000/100/6500    4231505 ms      4110262 ms            1

BENCHMARK_MAIN();