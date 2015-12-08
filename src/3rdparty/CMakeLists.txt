if(ENABLE_TESTS)
    add_subdirectory(googletest)
endif()

# Not building tests for protobuf
set(BUILD_TESTING OFF CACHE BOOL "")
add_subdirectory(protobuf/cmake)

# Custom build of grpc
add_subdirectory(cmake/grpc)

# Custom build of protobuf-qml
add_subdirectory(cmake/protobuf-qml)
