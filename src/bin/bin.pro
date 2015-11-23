TEMPLATE = app
TARGET = qml-runtime

QT += qml quick
CONFIG += c++11

INCLUDEPATH += ../lib
LIBS += -L../lib -lqml-runtime

SOURCES += \
    main.cpp
