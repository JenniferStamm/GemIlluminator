TEMPLATE = app

QT += qml quick widgets sensors multimedia

SOURCES += main.cpp \
    geometry.cpp \
    geometryrenderer.cpp \
    scene.cpp \
    scenerenderer.cpp \
    abstractnavigation.cpp \
    player.cpp \
    lightray.cpp \
    gem.cpp \
    gemrenderer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml
HEADERS += \
    geometry.h \
    geometryrenderer.h \
    scene.h \
    scenerenderer.h \
    abstractnavigation.h \
    player.h \
    lightray.h \
    gem.h \
    gemrenderer.h
