#include <benchmark/benchmark.h>

#include <memory>
#include <numeric>

#include <avl.hpp>
#include <bst.hpp>
#include <rb.hpp>
#include <shuffle.hpp>

namespace settings {
int iterations = 32;
} // namespace settings

// ------------------------------------------------------------------
template<typename T> struct user_counter;

// ------------------------------------------------------------------
template<>
struct user_counter<avl::node> {
   void operator () (const avl::node& type, benchmark::State& state) const {
      const auto& rc = type.get_rotation_counters();
      state.counters.insert({
         {"small_l", rc.small_left},
         {"small_r", rc.small_right},
         {"big_l",   rc.big_left},
         {"big_r",   rc.big_right}
      });
   }
};

// ------------------------------------------------------------------
template<>
struct user_counter<bst::node> {
   void operator () (const bst::node&, benchmark::State& state) const {
   state.counters.insert({
      {"small_l" , 0},
      {"small_r",  0},
      {"big_l",    0},
      {"big_r",    0}
      });
   }
};

// ------------------------------------------------------------------
template<typename TREE>
static void BM_increased_sequence_insertion(benchmark::State& state) {
   for (auto _ : state) {
      TREE t(0);
      for(int i = 1, sz = state.range(0); i < sz; ++i) {
         t.insert(i);
      }
      state.PauseTiming();
         user_counter<TREE> uc;
         uc(t, state);
      state.ResumeTiming();
   }
}
BENCHMARK_TEMPLATE(BM_increased_sequence_insertion, bst::node)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_TEMPLATE(BM_increased_sequence_insertion, avl::node)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

// ------------------------------------------------------------------
template<typename TREE>
static void BM_shuffled_values_insertion(benchmark::State& state) {

   const auto arr_sz = state.range(0);
   std::unique_ptr<int[]> arr(new int[arr_sz]);
   std::iota(arr.get(), arr.get() + arr_sz, 1);
   make_shuffle(arr.get(), arr_sz);

   for (auto _ : state) {
      TREE t(0);
      for(int i = 0; i < arr_sz; ++i) {
         t.insert(arr[i]);
      }
      state.PauseTiming();
         user_counter<TREE> uc;
         uc(t, state);
      state.ResumeTiming();
   }
}

BENCHMARK_TEMPLATE(BM_shuffled_values_insertion, bst::node)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_TEMPLATE(BM_shuffled_values_insertion, avl::node)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

// ------------------------------------------------------------------
template<typename T>
struct increased_tree_creator {
   explicit increased_tree_creator(size_t tree_sz) {
      for(int i = 1, sz = tree_sz; i < sz; ++i) {
         tree.insert(i);
      }
   }
   T tree{0};
};

// ------------------------------------------------------------------
template<typename T>
struct shuffled_tree_creator {
   explicit shuffled_tree_creator(size_t tree_sz) {
      std::unique_ptr<int[]> arr(new int[tree_sz]);
      std::iota(arr.get(), arr.get() + tree_sz, 1);
      make_shuffle(arr.get(), tree_sz);

      for(int i = 1, sz = tree_sz; i < sz; ++i) {
         tree.insert(arr.get()[i]);
      }
   }
   T tree{0};
};

// ------------------------------------------------------------------
template<typename T>
struct remove_call {
   void operator () (T& type, int value) const {
      type.remove(value);
   }
};

// ------------------------------------------------------------------
template<typename T>
struct search_call {
   void operator () (T& type, int value) const {
      type.search(value);
   }
};

// ------------------------------------------------------------------
template<typename TREE, typename CREATOR, typename CALL>
static void BM_tree_operations(benchmark::State& state) {

   const auto arr_sz = state.range(0);
   const auto call_sz = arr_sz / 10;

   std::mt19937 random_engine;
   std::uniform_int_distribution<int> distribution{0, static_cast<int>(arr_sz)};

   std::unique_ptr<int[]> call_data{new int[call_sz]};
   for (int i = 0; i < call_sz; ++i) {
      *(call_data.get() + i) = distribution(random_engine);
   }

   for (auto _ : state) {
      state.PauseTiming();
         CREATOR tc(arr_sz);
      state.ResumeTiming();

      for (int i = 0, sz = call_sz; i < sz; ++i) {
         CALL()(tc.tree, call_data[i]);
      }
   }
}

BENCHMARK_TEMPLATE(BM_tree_operations, bst::node, increased_tree_creator<bst::node>, remove_call<bst::node>)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_TEMPLATE(BM_tree_operations, bst::node, shuffled_tree_creator<bst::node>, remove_call<bst::node>)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_TEMPLATE(BM_tree_operations, avl::node, increased_tree_creator<avl::node>, remove_call<avl::node>)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_TEMPLATE(BM_tree_operations, avl::node, shuffled_tree_creator<avl::node>, remove_call<avl::node>)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_TEMPLATE(BM_tree_operations, bst::node, increased_tree_creator<bst::node>, search_call<bst::node>)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_TEMPLATE(BM_tree_operations, bst::node, shuffled_tree_creator<bst::node>, search_call<bst::node>)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_TEMPLATE(BM_tree_operations, avl::node, increased_tree_creator<avl::node>, search_call<avl::node>)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_TEMPLATE(BM_tree_operations, avl::node, shuffled_tree_creator<avl::node>, search_call<avl::node>)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);


// ------------------------------------------------------------------
static void BM_rb_tree_increased_sequence_insertion(benchmark::State& state) {
   const auto arr_sz = state.range(0);

   for (auto _ : state) {
      rb::tree t;
      for(int i = 0; i < arr_sz; ++i) {
         t.insert(i);
      }
   }
}
BENCHMARK(BM_rb_tree_increased_sequence_insertion)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

// ------------------------------------------------------------------
static void BM_rb_tree_shuffled_sequence_insertion(benchmark::State& state) {
   const auto tree_sz = state.range(0);
   std::unique_ptr<int[]> arr(new int[tree_sz]);
   std::iota(arr.get(), arr.get() + tree_sz, 0);
   make_shuffle(arr.get(), tree_sz);

   for (auto _ : state) {
      rb::tree t;
      for(int i = 0; i < tree_sz; ++i) {
         t.insert(arr.get()[i]);
      }
   }
}
BENCHMARK(BM_rb_tree_shuffled_sequence_insertion)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);


// ------------------------------------------------------------------
std::vector<int> get_random_digits(size_t distribution_sz, size_t out_sz) {
   std::vector<int> result(out_sz);

   std::mt19937 random_engine;
   std::uniform_int_distribution<int> distribution{0, static_cast<int>(distribution_sz)};

   std::generate(result.begin(), result.end(),
                 [&distribution, &random_engine](){return distribution(random_engine);});

   return result;
}

// ------------------------------------------------------------------
static void BM_rb_tree_increased_sequence_search(benchmark::State& state) {
   const auto tree_sz = state.range(0);

   rb::tree t;
   for(int i = 0; i < tree_sz; ++i) {
      t.insert(i);
   }

   const auto search_sz = tree_sz / 10;
   const auto for_search = get_random_digits(tree_sz, search_sz);

   for (auto _ : state) {
      for(int i = 0; i < search_sz; ++i) {
          t.search(for_search[i]);
      }
   }
}
BENCHMARK(BM_rb_tree_increased_sequence_search)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

// ------------------------------------------------------------------
static void BM_rb_tree_shuffled_sequence_search(benchmark::State& state) {
   const auto tree_sz = state.range(0);
   std::unique_ptr<int[]> arr(new int[tree_sz]);
   std::iota(arr.get(), arr.get() + tree_sz, 0);
   make_shuffle(arr.get(), tree_sz);

   rb::tree t;
   for(int i = 0; i < tree_sz; ++i) {
      t.insert(arr.get()[i]);
   }
   const auto search_sz = tree_sz / 10;
   const auto for_search = get_random_digits(tree_sz, search_sz);

   for (auto _ : state) {
      for(int i = 0; i < search_sz; ++i) {
         t.search(for_search[i]);
      }
   }
}
BENCHMARK(BM_rb_tree_shuffled_sequence_search)->Unit(benchmark::kMillisecond)->Iterations(settings::iterations)
      ->Arg(1'000)
      ->Arg(5'000)
      ->Arg(10'000)
      ->Arg(20'000)
      ->Arg(40'000);

BENCHMARK_MAIN();