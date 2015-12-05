/*
* Copyright (C) 2015 Siteshwar Vashisht <siteshwar@gmail.com>
*
* This library is free software; you can redistribute it and/or modify it
* under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
* License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#include "qmlmodule.h"
#include "protobuf/qml/descriptors.h"
#include "protobuf/qml/memory.h"
#include "protobuf/qml/file.h"
#include "protobuf/qml/method.h"
#include "protobuf/qml/server_method.h"
#include "grpc/qml/credentials.h"
#include "grpc/qml/server_credentials.h"
#include "grpc/qml/base.h"
#include "grpc/qml/server.h"

void QmlModule::registerTypes()
{
    registerProtobufTypes();
    registerGrpcTypes();
}

static QObject * descriptorPoolFactory(QQmlEngine*, QJSEngine*)
{
    return new protobuf::qml::DescriptorPoolWrapper;
}

void QmlModule::registerProtobufTypes()
{
    qmlRegisterSingletonType<protobuf::qml::DescriptorPoolWrapper>("Protobuf", 1, 0, "DescriptorPool", descriptorPoolFactory);
    qmlRegisterUncreatableType<protobuf::qml::FileDescriptorWrapper>("Protobuf", 1, 0, "FileDescriptor", QLatin1String("FileDescriptor is abstract type."));
    qmlRegisterUncreatableType<protobuf::qml::DescriptorWrapper>("Protobuf", 1, 0, "Descriptor", QLatin1String("Descriptor is abstract type."));
    qmlRegisterUncreatableType<protobuf::qml::Descriptor>("Protobuf", 1, 0, "V4Descriptor", QLatin1String("Descriptor is abstract type."));
    qmlRegisterUncreatableType<protobuf::qml::StatusCode>("Protobuf", 1, 0, "StatusCode", QLatin1String("StatusCode is enum holder."));

    // buffers
    qmlRegisterType<protobuf::qml::MemoryBufferChannel>("Protobuf", 1, 0, "MemoryBufferChannel");
    qmlRegisterType<protobuf::qml::FileChannel>("Protobuf", 1, 0, "FileChannel");

    // client
    qmlRegisterType<protobuf::qml::Channel2>("Protobuf", 1, 0, "Channel2");
    qmlRegisterType<protobuf::qml::UnaryMethodHolder>("Protobuf", 1, 0, "UnaryMethodHolder");
    qmlRegisterType<protobuf::qml::WriterMethodHolder>("Protobuf", 1, 0, "WriterMethodHolder");
    qmlRegisterType<protobuf::qml::ReaderMethodHolder>("Protobuf", 1, 0, "ReaderMethodHolder");
    qmlRegisterType<protobuf::qml::ReaderWriterMethodHolder>("Protobuf", 1, 0, "ReaderWriterMethodHolder");

    // server
    qmlRegisterType<protobuf::qml::RpcServer>("Protobuf", 1, 0, "RpcServer");
    qmlRegisterType<protobuf::qml::RpcService>("Protobuf", 1, 0, "RpcService");
    qmlRegisterType<protobuf::qml::ServerUnaryMethodHolder>("Protobuf", 1, 0, "ServerUnaryMethodHolder");
    qmlRegisterType<protobuf::qml::ServerReaderMethodHolder>("Protobuf", 1, 0, "ServerReaderMethodHolder");
    qmlRegisterType<protobuf::qml::ServerWriterMethodHolder>("Protobuf", 1, 0, "ServerWriterMethodHolder");
    qmlRegisterType<protobuf::qml::ServerReaderWriterMethodHolder>("Protobuf", 1, 0, "ServerReaderWriterMethodHolder");
}

void QmlModule::registerGrpcTypes()
{
    qmlRegisterType<grpc::qml::Channel>("Grpc", 1, 0, "GrpcChannel");
    qmlRegisterType<grpc::qml::InsecureCredentials>("Grpc", 1, 0,"InsecureCredentials");
    qmlRegisterUncreatableType<grpc::qml::Credentials>("Grpc", 1, 0, "Credentials", QLatin1String("Credentials is base type that cannot be instantiated itself."));

    qmlRegisterType<grpc::qml::GrpcServer>("Grpc", 1, 0, "GrpcServer");
    qmlRegisterType<grpc::qml::InsecureServerCredentials>("Grpc", 1, 0, "InsecureServerCredentials");
    qmlRegisterUncreatableType<grpc::qml::ServerCredentials>("Grpc", 1, 0, "ServerCredentials", QLatin1String("Credentials is base type that cannot be instantiated itself."));
}
