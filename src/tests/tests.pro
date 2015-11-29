TEMPLATE = app
TARGET = tests

QT += qml quick testlib
CONFIG += c++11

INCLUDEPATH += ../lib
LIBS += -L../lib -lqml-runtime

include(../common.pri)
include(gtest.pri)

HEADERS += \
    mocknetworkaccessmanager.h \
    mockurlinterceptor.h

SOURCES += \
    main.cpp \
    tst_urlinterceptor.cpp \
    tst_networkaccessmanager.cpp \
    tst_qmlruntime.cpp

RESOURCES += \
    res.qrc

OTHER_FILES += \
    preload.qml \
    preload-fail.qml
