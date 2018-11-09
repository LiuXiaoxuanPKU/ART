cmake_minimum_required (VERSION 2.6)
project (ART)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -mpopcnt -pthread -std=c++11")

enable_testing()

include_directories ("${PROJECT_SOURCE_DIR}/include")

add_subdirectory (src)
add_subdirectory (test)
