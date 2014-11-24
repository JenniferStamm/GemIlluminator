TEMPLATE = app

QT += qml quick widgets sensors #multimedia

SOURCES += main.cpp \
    scene.cpp \
    scenerenderer.cpp \
    abstractnavigation.cpp \
    player.cpp \
    lightray.cpp \
    gem.cpp \
    gemrenderer.cpp \
    abstractgeometry.cpp \
    abstractgeometryrenderer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

QMAKE_CXXFLAGS += -std=c++0x

OTHER_FILES += \
    android/AndroidManifest.xml
HEADERS += \
    scene.h \
    scenerenderer.h \
    abstractnavigation.h \
    player.h \
    lightray.h \
    gem.h \
    gemrenderer.h \
    abstractgeometryrenderer.h \
    abstractgeometry.h
