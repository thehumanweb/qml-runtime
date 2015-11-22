TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp qmlruntime.cpp ipfsonlyurlinterceptor.cpp customnetworkaccessmanager.cpp

HEADERS += qmlruntime.h ipfsonlyurlinterceptor.h customnetworkaccessmanager.h

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

