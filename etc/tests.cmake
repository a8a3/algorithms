
add_executable(etc tests.cpp)

target_include_directories(etc
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../tests/include
)

if (MSVC)
    target_compile_options(etc PRIVATE /W4 )
else ()
    target_compile_options(etc PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
