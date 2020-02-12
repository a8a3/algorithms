
add_executable(file_tester ./file_tests/tester.cpp)

set_target_properties(file_tester PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_compile_options(file_tester
        PRIVATE
        -O3
        -Wpedantic
        -Wall
        -Wextra
        -Werror
)

target_include_directories(file_tester
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/
)

target_link_libraries(file_tester
        PRIVATE
        -lstdc++fs  # gcc < 9+ requirements for std::filesystem usage
)
