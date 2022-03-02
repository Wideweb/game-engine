cmake_minimum_required(VERSION 3.13)

project(GameEngine VERSION 1.0)

add_compile_options(
    # -Werror
    # -pedantic-errors

    # -Wall
    # -Wextra
    # -Wpedantic

    # -Wcast-align
    # -Wcast-qual
    # -Wconversion
    # -Wctor-dtor-privacy
    # -Wextra-semi
    # -Wfloat-equal
    # -Wnon-virtual-dtor
    # -Wold-style-cast
    # -Woverloaded-virtual
    # -Wredundant-decls
    # -Wsign-conversion
    # -Wsign-promo
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_UNITY_BUILD ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

add_subdirectory(engine)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game/main.lua DESTINATION ${OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/game/assets DESTINATION ${OUTPUT_DIRECTORY})