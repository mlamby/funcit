cmake_minimum_required (VERSION 2.8)
project (funcit)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")
endif()

include_directories("${PROJECT_BINARY_DIR}")

add_executable (funcit test.cpp funcit.h)

include (CTest)
add_test (ItRuns funcit)
