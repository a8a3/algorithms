
find_package(benchmark REQUIRED)

add_executable(simple_data_structures_benchmarks benchmarks.cpp)

target_link_libraries(simple_data_structures_benchmarks
     PRIVATE benchmark::benchmark
)

if (MSVC)
     target_compile_options(simple_data_structures_benchmarks PRIVATE /W4 /MD)
else ()
     target_compile_options(simple_data_structures_benchmarks PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
