
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

if (MSVC)
    target_compile_options(algebraic PRIVATE /W4 -O3)
else ()
    target_compile_options(algebraic PRIVATE -Wall -Wextra -pedantic -Werror -O3)
endif()

target_link_libraries(algebraic
        PRIVATE
        -lstdc++fs  # gcc < 9+ requirements for std::filesystem usage
)
