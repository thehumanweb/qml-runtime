project(qml-runtime-lib)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_EXTRA_WARNINGS_FLAGS}")

find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

include_directories(
    ${QT_INCLUDES}
)

set(${PROJECT_NAME}_SRCS
    qobjectptr.h
    ilockableurlinterceptor.h
    ipfsonlyurlinterceptor.cpp
    customnetworkaccessmanager.cpp
    qmlruntime.cpp
    qmlruntimefactory.cpp
    qmlmodule.cpp
)

qt5_add_resources(${PROJECT_NAME}_Res_SRCS
    preload.qrc
)

add_library(${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_SRCS}
    ${${PROJECT_NAME}_Res_SRCS}
)
qt5_use_modules(${PROJECT_NAME} Core Qml Quick)
target_link_libraries(${PROJECT_NAME}
    protobuf-qml-lib
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}
)
