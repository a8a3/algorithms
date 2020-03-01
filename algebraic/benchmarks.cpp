#include <benchmark/benchmark.h>

#include "include/fib.hpp"
#include "include/pow.hpp"

// ------------------------------------------------------------------
static void BM_recursive_fib(benchmark::State& state) {
    for (auto _ : state) {
        recursive_fib(state.range(0));
    }
}
BENCHMARK(BM_recursive_fib)->Iterations(10)->RangeMultiplier(2)->Range(2, 32);

// ------------------------------------------------------------------
static void BM_cycle_fib(benchmark::State& state) {
    for (auto _ : state) {
        cycle_fib(state.range(0));
    }
}
BENCHMARK(BM_cycle_fib)->RangeMultiplier(10)->Range(100'000, 10'000'000);

static void BM_golden_ratio_fib(benchmark::State& state) {
    for (auto _ : state) {
        golden_ratio_fib(state.range(0));
    }
}
BENCHMARK(BM_golden_ratio_fib)->RangeMultiplier(10)->Range(100'000, 10'000'000);

static void BM_fib_matrix(benchmark::State& state) {
    for (auto _ : state) {
        fib_matrix::get_value(state.range(0));
    }
}
BENCHMARK(BM_fib_matrix)->RangeMultiplier(10)->Range(100'000, 10'000'000);

namespace BM_pow {
constexpr static double base = 1.000001;
// ------------------------------------------------------------------
static void BM_naive_pow(benchmark::State& state) {
    for (auto _ : state) {
        naive_pow(base, static_cast<uint32_t>(state.range(0)));
    }
}
BENCHMARK(BM_naive_pow)->RangeMultiplier(10)->Range(100'000, 10'000'000);

static void BM_use_pow_of_2(benchmark::State& state) {
    for (auto _ : state) {
        use_pow_of_2(base, static_cast<uint32_t>(state.range(0)));
    }
}
BENCHMARK(BM_use_pow_of_2)->RangeMultiplier(10)->Range(100'000, 10'000'000);

static void BM_use_pow_of_p(benchmark::State& state) {
    for (auto _ : state) {
        use_pow_of_p(base, static_cast<uint32_t>(state.range(0)));
    }
}
BENCHMARK(BM_use_pow_of_p)->RangeMultiplier(10)->Range(100'000, 10'000'000);
} // namespace pow

BENCHMARK_MAIN();