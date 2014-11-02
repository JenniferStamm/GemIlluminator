TEMPLATE = app

QT += qml quick widgets sensors opengl multimedia

SOURCES += main.cpp \
    cube.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

include(deployment.pri)

HEADERS += \
    cube.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml
