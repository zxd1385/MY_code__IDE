cmake_minimum_required(VERSION 3.29)
project(MY_code)

set(CMAKE_CXX_STANDARD 20)

add_executable(MY_code main.cpp)
target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
