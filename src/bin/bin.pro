TEMPLATE = app
TARGET = qml-runtime

include(../common.pri)

QT += qml quick
CONFIG += c++11

INCLUDEPATH += ../lib
LIBS += -L../lib -lqml-runtime

SOURCES += \
    main.cpp
