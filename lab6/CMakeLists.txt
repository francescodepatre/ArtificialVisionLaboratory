cmake_minimum_required(VERSION 3.6)
project(lab4_2)

find_package( OpenCV REQUIRED )
#include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(/opt/homebrew/include)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab4_2 main.cpp)

target_link_libraries( lab4_2 ${OpenCV_LIBS} )
install(TARGETS lab4_2 RUNTIME DESTINATION bin)
