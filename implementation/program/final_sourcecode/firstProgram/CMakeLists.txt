cmake_minimum_required(VERSION 2.8)
project( TextSeg )

file(GLOB SOURCES "src/*.cpp")
find_package( OpenCV REQUIRED )

set (CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS "-std=c++0x")

include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories(include)

add_executable( TextSeg ${SOURCES})
target_link_libraries( TextSeg ${OpenCV_LIBS} )

