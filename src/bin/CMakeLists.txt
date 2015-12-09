project(qml-runtime)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_EXTRA_WARNINGS_FLAGS} ${CMAKE_CXX_COVERAGE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

find_library(OPENSSL_SSL_LIBRARY NAMES ssl)
mark_as_advanced(OPENSSL_SSL_LIBRARY)

find_library(OPENSSL_CRYPTO_LIBRARY NAMES crypto)
mark_as_advanced(OPENSSL_CRYPTO_LIBRARY)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

include_directories(
    ${QT_INCLUDES}
)

set(${PROJECT_NAME}_SRCS
    main.cpp
)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_SRCS}
)
qt5_use_modules(${PROJECT_NAME} Core Gui Network Qml Quick)
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Quick
    qml-runtime-lib
)

