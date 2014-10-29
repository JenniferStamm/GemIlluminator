TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    squircle.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

include(deployment.pri)

HEADERS += \
    squircle.h
