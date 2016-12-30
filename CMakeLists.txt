cmake_minimum_required(VERSION 3.0)

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_MODULE_PATH}")
#MESSAGE(STATUS "CMAKE_MODULE_PATH : " ${CMAKE_MODULE_PATH})

project(gtkcaffecv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-sign-compare -Wno-unused-function -Wno-uninitialized")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-attributes -Wno-deprecated-declarations -D_USE_MATH_DEFINES")

# ---[ Using cmake scripts and modules

option(USE_OPENCL "Build gtkcaffecv with OpenCL support" ON) 

if("${USE_OPENCL}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_OPENCL -DUSE_GREENTEA")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCPU_ONLY")
endif()

# ---[ Configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Possible configurations" FORCE)
mark_as_advanced(CMAKE_CONFIGURATION_TYPES)

if(DEFINED CMAKE_BUILD_TYPE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

# --[ If user doesn't specify build type then assume release
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)
endif()

MESSAGE(STATUS "This is SOURCE dir " ${CMAKE_SOURCE_DIR})
MESSAGE(STATUS "CMAKE_CXX_FLAGS :  " ${CMAKE_CXX_FLAGS})
MESSAGE(STATUS "CMAKE_BUILD_TYPE : " ${CMAKE_BUILD_TYPE})

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CMAKE_CXX_FLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CMAKE_CXX_FLAGS} -O2 -Wall")

set(Gtkcaffecv_LINKER_LIBS)

include_directories("${CMAKE_SOURCE_DIR}/src")
include_directories("${CMAKE_SOURCE_DIR}/src/mcvcaffe")
include_directories("${CMAKE_SOURCE_DIR}/src/mcvwidget")

find_package(Boost 1.58.0 REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND Gtkcaffecv_LINKER_LIBS ${Boost_LIBRARIES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)
include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
list(APPEND Gtkcaffecv_LINKER_LIBS ${GTKMM_LIBRARIES})

# SET(OpenCV_DIR "C:/OpenCV")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
list(APPEND Gtkcaffecv_LINKER_LIBS ${OpenCV_LIBRARIES})

# find OpenCL
find_package(OpenCL REQUIRED)
include_directories(SYSTEM ${OPENCL_INCLUDE_DIRS})
list(APPEND Gtkcaffecv_LINKER_LIBS ${OPENCL_LIBRARIES})

find_package(Caffe REQUIRED)
#include_directories(${Caffe_INCLUDE_DIRS})
#link_directories(${Caffe_LIBRARY_DIRS})
list(APPEND Gtkcaffecv_LINKER_LIBS ${Caffe_LIBRARIES})

MESSAGE(STATUS "Caffe_INCLUDE_DIRS : " ${Caffe_INCLUDE_DIRS})
MESSAGE(STATUS "Caffe_LIBRARIES : " ${Caffe_LIBRARIES})

INCLUDE_DIRECTORIES("/mingw/include")
#INCLUDE_DIRECTORIES("/mingw64/include/OpenBLAS/")
INCLUDE_DIRECTORIES("C:/msys64/mingw64/include/OpenBLAS")
#LINK_DIRECTORIES("/mingw/lib") 

file(GLOB_RECURSE  mcvcaffe_source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/mcvcaffe/*.cc")
ADD_LIBRARY(mcvcaffe ${mcvcaffe_source_files} )

file(GLOB_RECURSE mcvwidget_source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/mcvwidget/*.cc")
ADD_LIBRARY(mcvwidget ${mcvwidget_source_files} )

file(GLOB_RECURSE source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")
add_executable(gtkcaffecv ${source_files})
target_link_libraries(gtkcaffecv mcvwidget mcvcaffe ${Gtkcaffecv_LINKER_LIBS})



