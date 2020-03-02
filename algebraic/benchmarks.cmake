
find_package(benchmark REQUIRED)

add_executable(algebraic_benchmarks benchmarks.cpp)

target_link_libraries(algebraic_benchmarks
     PRIVATE benchmark::benchmark
)

if (MSVC)
     target_compile_options(algebraic_benchmarks PRIVATE /W4 /MD)
else ()
     target_compile_options(algebraic_benchmarks PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
