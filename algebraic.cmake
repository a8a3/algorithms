
add_executable(algebraic ${CMAKE_CURRENT_SOURCE_DIR}/tests/algebraic.cpp)

set_target_properties(algebraic PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_include_directories(algebraic
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/algebraic
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/include
)

target_compile_options(algebraic
        PRIVATE
        -O3
        -Wpedantic
        -Wall
        -Wextra
        -Werror
)

target_link_libraries(algebraic
        PRIVATE
        -lstdc++fs  # gcc < 9+ requirements for std::filesystem usage
)
