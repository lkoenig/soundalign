project(soundalign)
cmake_minimum_required(VERSION 3.0)

# Options
if(CMAKE_BUILD_TYPE MATCHES "Release")
    set(RELEASE_OFF OFF)
else()
    set(RELEASE_OFF ON)
endif()

option(USE_ADDRESS_SANITIZER "Using Address sanitizer" ${RELEASE_OFF})
option(DEBUG_DUMP_DATA "Dump debug data" OFF)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(OpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

# Required packages
find_package(Eigen3 REQUIRED)
find_package(LibSndFile REQUIRED)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if(DEBUG_DUMP_DATA)
    add_definitions(-DDEBUG_DUMP_DATA=1)
endif(DEBUG_DUMP_DATA)

set(COMMON_FLAGS "")

# Use Address Sanitizer if the compiler allows it
if(USE_ADDRESS_SANITIZER AND (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX))
    set(COMMON_FLAGS "-fsanitize=address ${COMMON_FLAGS}")
endif()

# Ignoring int-in-bool-context warning as Eigen uses it a lot
set(COMMON_FLAGS "-Wno-int-in-bool-context ${COMMON_FLAGS}")

# Treat all warnings as errors
set(COMMON_FLAGS "-Wall -Werror ${COMMON_FLAGS}")

set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${CMAKE_CXX_FLAGS}")

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${LIBSNDFILE_INCLUDE_DIR})
include_directories(kiss_fft)


include_directories(src)

add_subdirectory(kiss_fft)
add_subdirectory(src)

