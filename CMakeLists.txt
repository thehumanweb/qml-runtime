project(qml-runtime)
cmake_minimum_required(VERSION 2.8.11)

# Options
option(ENABLE_TESTS "Enable tests and coverage" OFF)

# Configuration
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_SILENT_WARNINGS_FLAGS "-Wno-unused-parameter -Wno-sign-compare")
set(CMAKE_CXX_EXTRA_WARNINGS_FLAGS "-Wunused-parameter -Wsign-compare")
# protobuf spits too much of these
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_SILENT_WARNINGS_FLAGS}")

if(ENABLE_TESTS)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        message("Building with tests and coverage")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
    endif(CMAKE_BUILD_TYPE MATCHES Debug)
endif(ENABLE_TESTS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Projects
enable_testing()
add_subdirectory(src/3rdparty)
add_subdirectory(src/lib)
add_subdirectory(src/bin)
if(ENABLE_TESTS)
    add_subdirectory(src/tests)
endif(ENABLE_TESTS)
 
