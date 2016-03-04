project(soundalign)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# Required packages
find_package(Eigen3 REQUIRED)
find_package(LibSndFile REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${LIBSNDFILE_INCLUDE_DIR})
include_directories(kiss_fft)


include_directories(src)

add_subdirectory(kiss_fft)
add_subdirectory(src)

