project(protobuf-qml-compiler)

include_directories(
    ${PROTOBUF-QML_DIR}/compiler
)

set(${PROJECT_NAME}_Compiler_SRCS
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/util.cpp
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/enum_generator.cpp
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/field_generator.cpp
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/message_generator.cpp
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/oneof_generator.cpp
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/service_generator.cpp
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/qml_generator.cpp
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/compiler_util.cpp
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/main.cpp

    # Qt dependency
    ${PROTOBUF-QML_DIR}/compiler/protobuf/qml/base64.cpp
)

add_executable(protoc-gen-qml ${${PROJECT_NAME}_Compiler_SRCS})
qt5_use_modules(protoc-gen-qml Core)
target_link_libraries(protoc-gen-qml
    libprotoc
    protobuf-qml-lib
)
