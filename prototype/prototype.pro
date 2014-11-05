TEMPLATE = app

QT += opengl qml quick widgets sensors multimedia gui

SOURCES += main.cpp \
    cube.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

INCLUDEPATH +=

include(deployment.pri)

HEADERS += \
    cube.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml
