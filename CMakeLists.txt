cmake_minimum_required(VERSION 3.6.3)

set(CMAKE_DISABLE_IN_SOURCE_BUILD   ON)
set(CMAKE_DISABLE_SOURCE_CHANGES    ON)

project(jhttpd CXX)

set(JHTTPD_VERSION_MAJOR 0)
set(JHTTPD_VERSION_MINOR 0)
set(JHTTPD_VERSION_PATCH 1)
set(JHTTPD_VERSION_SOVER 1)


if(OVERRIDE_CXX_STANDARD_FLAGS)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_EXTENSIONS Off)
endif()


option(BUILD_JHTTPD_SHARED_LIBS "Build libraries as shared ones" ON)
option(BUILD_JHTTPD_DEMO "Build libraries as shared ones" ON)


#set unified output path
set(JHTTPD_OUT_DIR  ${CMAKE_BINARY_DIR}/output)

message(STATUS "OUTPUT DIRECTORY: ${JHTTPD_OUT_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${JHTTPD_OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${JHTTPD_OUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${JHTTPD_OUT_DIR})



add_subdirectory(src)

if(BUILD_JHTTPD_DEMO)
    add_subdirectory(demo)
    set_property(TARGET jhttpd      PROPERTY FOLDER "jhttpd")
    set_property(TARGET jhttpd_demo PROPERTY FOLDER "jhttpd")
endif()
