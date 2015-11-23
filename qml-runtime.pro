TEMPLATE = app
TARGET = qml-runtime

QT += qml quick
CONFIG += c++11

HEADERS += \
    qmlruntime.h \
    ipfsonlyurlinterceptor.h \
    customnetworkaccessmanager.h \
    qobjectptr.h

SOURCES += \
    main.cpp \
    qmlruntime.cpp \
    ipfsonlyurlinterceptor.cpp \
    customnetworkaccessmanager.cpp

RESOURCES += \
    preload.qrc

OTHER_FILES += \
    preload.qml
