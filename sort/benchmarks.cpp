#include <benchmark/benchmark.h>

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


//Как сравнивать скорость.
//1. Выбрать подходящий размер массива, чтобы алгоритм работал 5-20 секунд.
//2. Создать два массива выбранного размера:
//а. случайный;
//б. отсортированный, в котором перемешано 5% элементов.
//3. Замерять время работы алгоритмов HeapSort и ShellSort (с тремя разными вариантами шагов)
//для каждого массива, заполняя табличку. Таблицу приложить к коду.
//4. Сделать вывод, какой алгоритм соритровки лучше.

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


BENCHMARK_MAIN();