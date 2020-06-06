
set(TARGET_NAME dynamic_tests)
add_executable(${TARGET_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/tests.cpp
        )

target_include_directories(${TARGET_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../tests/include
        )

if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 )
else ()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
