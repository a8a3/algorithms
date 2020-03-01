
add_executable(simple_data_structures ${CMAKE_CURRENT_SOURCE_DIR}/tests/simple_data_structures.cpp)

set_target_properties(simple_data_structures PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
)

target_include_directories(simple_data_structures
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/algorithms/simple_data_structures
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/include
)

if (MSVC)
    target_compile_options(simple_data_structures PRIVATE /W4 )
else ()
    target_compile_options(simple_data_structures PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
