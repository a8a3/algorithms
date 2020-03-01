
add_executable(algebraic_tests tests.cpp)

target_include_directories(algebraic_tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../tests/include
)

if (MSVC)
    target_compile_options(algebraic_tests PRIVATE "/W4")
else ()
    target_compile_options(algebraic_tests PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
