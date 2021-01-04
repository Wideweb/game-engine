cmake_minimum_required(VERSION 3.13)

project(App VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_EXE 
    src/main.cpp
)	

add_executable(game ${SOURCE_EXE})

target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(game PRIVATE engine)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${OUTPUT_DIRECTORY})

