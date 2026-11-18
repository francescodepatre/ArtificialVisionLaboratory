cmake_minimum_required(VERSION 3.6)
project(lab4)

find_package(OpenCV REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(lab5 main.cpp)

target_link_libraries(lab5 ${OpenCV_LIBS} Eigen3::Eigen)

install(TARGETS lab5 RUNTIME DESTINATION bin)