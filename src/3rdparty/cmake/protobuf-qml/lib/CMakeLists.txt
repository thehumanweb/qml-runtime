project(protobuf-qml-lib)

include_directories(
    ${PROTOBUF-QML_DIR}/compiler
)

set(${PROJECT_NAME}_Protobuf_SRCS
    ${PROTOBUF-QML_DIR}/lib/protobuf/qml/descriptors.cpp
    ${PROTOBUF-QML_DIR}/lib/protobuf/qml/file.cpp
    ${PROTOBUF-QML_DIR}/lib/protobuf/qml/memory.cpp
    ${PROTOBUF-QML_DIR}/lib/protobuf/qml/buffer.cpp
    ${PROTOBUF-QML_DIR}/lib/protobuf/qml/method.cpp
    ${PROTOBUF-QML_DIR}/lib/protobuf/qml/server_method.cpp
    ${PROTOBUF-QML_DIR}/lib/protobuf/qml/conversions.cpp
    ${PROTOBUF-QML_DIR}/lib/protobuf/qml/v4/descriptor.cpp
    ${PROTOBUF-QML_DIR}/lib/protobuf/qml/v4/v4util.cpp
)

set(${PROJECT_NAME}_Grpc_SRCS
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/server_unary.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/server_reader.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/server_writer.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/server_bidi.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/server.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/server_credentials.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/credentials.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/base.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/unary.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/writer.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/reader.cpp
    ${PROTOBUF-QML_DIR}/lib/grpc/qml/reader_writer.cpp
)

add_library(${PROJECT_NAME} STATIC
    ${${PROJECT_NAME}_Protobuf_SRCS}
    ${${PROJECT_NAME}_Protobuf_Plugin_SRCS}
    ${${PROJECT_NAME}_Grpc_SRCS}
    ${${PROJECT_NAME}_Grpc_Plugin_SRCS}
)
qt5_use_modules(${PROJECT_NAME} Quick)
target_link_libraries(${PROJECT_NAME}
    grpc++
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${QT_INCLUDES}
    ${PROTOBUF-QML_DIR}/lib
    ${Qt5Qml_PRIVATE_INCLUDE_DIRS}
)
