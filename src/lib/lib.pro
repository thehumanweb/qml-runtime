TEMPLATE = lib
TARGET = qml-runtime
CONFIG += static

include(../common.pri)

QT += qml quick
CONFIG += c++11

HEADERS += \
    qmlruntime.h \
    ipfsonlyurlinterceptor.h \
    customnetworkaccessmanager.h \
    qobjectptr.h \
    ilockableurlinterceptor.h \
    qmlruntimefactory.h

SOURCES += \
    qmlruntime.cpp \
    ipfsonlyurlinterceptor.cpp \
    customnetworkaccessmanager.cpp \
    qmlruntimefactory.cpp

RESOURCES += \
    preload.qrc

OTHER_FILES += \
    preload.qml
