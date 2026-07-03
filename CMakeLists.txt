cmake_minimum_required(VERSION 3.20)

project(TRM
    VERSION 0.2.0
    LANGUAGES CXX
)

# ==========================
# C++ Standard
# ==========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================
# Executable
# ==========================
add_executable(TRM
    source/main.cpp
    source/core/shell.cpp
)

# ==========================
# Include Directories
# ==========================
target_include_directories(TRM
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# ==========================
# Compiler Warnings
# ==========================
if(MSVC)

    target_compile_options(TRM PRIVATE
        /W4
        /WX
        /EHsc
    )

else()

    target_compile_options(TRM PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
        -Wconversion
    )

endif()

# ==========================
# Windows Libraries
# ==========================
if(WIN32)

    target_link_libraries(TRM PRIVATE
        ws2_32
    )

endif()