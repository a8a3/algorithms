
set(TARGET_NAME codeforces)
add_executable(${TARGET_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/codeforces.cpp
        )

target_include_directories(${TARGET_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

if (MSVC)
    target_compile_options(${TARGET_NAME} PRIVATE /W4 )
else ()
    target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
