
add_executable(range range.cpp)

target_include_directories(range
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/range
    ${CMAKE_CURRENT_SOURCE_DIR}/../tests/include
)

if (MSVC)
    target_compile_options(range PRIVATE /W4 )
else ()
    target_compile_options(range PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
