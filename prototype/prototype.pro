TEMPLATE = app

QT += opengl qml quick widgets sensors multimedia gui

SOURCES += main.cpp \
    cube.cpp \
    scene.cpp \
    scenerenderer.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

INCLUDEPATH +=

include(deployment.pri)

HEADERS += \
    cube.h \
    scene.h \
    scenerenderer.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml
