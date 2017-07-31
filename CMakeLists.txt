cmake_minimum_required(VERSION 3.6)
project(BulkLoadRTree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -g")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")


find_package(SpatialIndex REQUIRED)

set(SOURCE_FILES main.cpp Point.h Point.cpp MyVisitor.h MyDataStream.h)
add_executable(BulkLoadRTree ${SOURCE_FILES})

target_link_libraries(BulkLoadRTree ${SPATIALINDEX_LIBRARIES} )