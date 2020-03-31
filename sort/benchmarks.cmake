
set(TARGET_NAME sort_benchmarks)
find_package(benchmark REQUIRED)

add_executable(${TARGET_NAME} benchmarks.cpp)

target_include_directories(${TARGET_NAME}
     PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../tests/include
)

target_link_libraries(${TARGET_NAME}
     PRIVATE benchmark::benchmark
)

if (MSVC)
     target_compile_options(${TARGET_NAME} PRIVATE /W4 /MD)
else ()
     target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
