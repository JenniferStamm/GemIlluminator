TEMPLATE = app

QT += qml quick widgets sensors #multimedia

SOURCES += main.cpp \
    scene.cpp \
    scenerenderer.cpp \
    player.cpp \
    lightray.cpp \
    gem.cpp \
    gemrenderer.cpp \
    camera.cpp \
    navigation.cpp \
    abstractgem.cpp \
    abstractgemrenderer.cpp

RESOURCES += qml.qrc \
    shader.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

CONFIG += c++11

OTHER_FILES += \
    android/AndroidManifest.xml
HEADERS += \
    scene.h \
    scenerenderer.h \
    player.h \
    lightray.h \
    gem.h \
    gemrenderer.h \
    camera.h \
    navigation.h \
    abstractgem.h \
    abstractgemrenderer.h
