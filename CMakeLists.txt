cmake_minimum_required(VERSION 3.10)
project(PaperDemo)

include_directories ("${PROJECT_SOURCE_DIR}/include")
include_directories ("${PROJECT_SOURCE_DIR}/components/")

set(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS  "-g --std=c++11 -W -Wall -Wpedantic" )

add_executable(PaperDemoFlat1 main/main.cpp)
set_target_properties(PaperDemoFlat1 PROPERTIES
        COMPILE_FLAGS "-DDEVICE_FLAT_NUM=1 -DNUM_FLATS=3 -DDEVICE_FLAT_ROOM=bedroom -DDEVICE_FLAT_AGGREGATOR_FOR=pressure -DSUPPRESS_NO_SUBSCRIBERS_LOG -DBUILDING_AGGREGATOR_FOR=temperature")

add_executable(PaperDemoFlat2 main/main.cpp)
set_target_properties(PaperDemoFlat2 PROPERTIES
        COMPILE_FLAGS "-DDEVICE_FLAT_NUM=2 -DNUM_FLATS=3 -DDEVICE_FLAT_ROOM=kitchen -DDEVICE_FLAT_AGGREGATOR_FOR=pressure -DSUPPRESS_NO_SUBSCRIBERS_LOG -DBUILDING_AGGREGATOR_FOR=pressure")


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

TARGET_LINK_LIBRARIES(PaperDemoFlat1 LINK_PUBLIC Threads::Threads)
TARGET_LINK_LIBRARIES(PaperDemoFlat2 LINK_PUBLIC Threads::Threads)
