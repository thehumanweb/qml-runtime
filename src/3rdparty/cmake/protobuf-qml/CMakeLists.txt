cmake_minimum_required(VERSION 2.8.12)
project(protobuf-qml)

set(PROTOBUF-QML_DIR ${CMAKE_SOURCE_DIR}/src/3rdparty/protobuf-qml)

set(CMAKE_AUTOMOC TRUE)

find_package(Qt5Qml REQUIRED PRIVATE)
find_package(Qt5Quick REQUIRED)

#HACK: CMake with broken Qt5Qml_PRIVATE_INCLUDE_DIRS, Qt5Quick_PRIVATE_INCLUDE_DIRS
if(${Qt5Qml_FOUND})
  if(NOT "${Qt5Qml_PRIVATE_INCLUDE_DIRS}" MATCHES "/QtQml/")
    string(REPLACE "/QtCore" "/QtQml" replaceme "${Qt5Core_PRIVATE_INCLUDE_DIRS}")
    list(APPEND Qt5Qml_PRIVATE_INCLUDE_DIRS ${replaceme})
    list(REMOVE_DUPLICATES Qt5Qml_PRIVATE_INCLUDE_DIRS)
  endif()
endif()

add_subdirectory(lib)
#add_subdirectory(compiler)
