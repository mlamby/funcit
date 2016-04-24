cmake_minimum_required (VERSION 2.6)
project (funcit)
enable_testing()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)

include_directories("${PROJECT_BINARY_DIR}")

add_executable (funcit test.cpp funcit.h)

include (CTest)
add_test (ItRuns funcit)
