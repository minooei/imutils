cmake_minimum_required(VERSION 3.8)
project(imutils)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(SOURCE_FILES main.cpp util.cpp util.h)
add_executable(imutils ${SOURCE_FILES})
target_link_libraries(imutils ${OpenCV_LIBRARIES})
