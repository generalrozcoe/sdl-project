cmake_minimum_required(VERSION 3.5.0)
project(sdl VERSION 0.1.0 LANGUAGES C CXX)
add_executable(sdl main.cpp snake.cpp  ) 
target_link_libraries(sdl  -lSDL2 -lSDL2_image -lSDL2_ttf )
target_compile_features(sdl PRIVATE cxx_std_11)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)