TEMPLATE = app

QT += qml quick widgets sensors #multimedia

SOURCES += main.cpp \
    scene.cpp \
    scenerenderer.cpp \
    player.cpp \
    lightray.cpp \
    gemrenderer.cpp \
    camera.cpp \
    lightrayrenderer.cpp \
    navigation.cpp \
    abstractgem.cpp \
    abstractgemrenderer.cpp \
    lightraydata.cpp \
    triangle.cpp \
    tetrahedrongem.cpp \
    cubegem.cpp \
    screenalignedquad.cpp \
    scenebounds.cpp \
    painter.cpp \
    painterqml.cpp \
    gemrenderernew.cpp \
    gemdata.cpp

RESOURCES += qml.qrc \
    shader.qrc \
    data.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

CONFIG += \
    c++11 \
    warn_on

OTHER_FILES += \
    android/AndroidManifest.xml
HEADERS += \
    scene.h \
    scenerenderer.h \
    player.h \
    lightray.h \
    gemrenderer.h \
    camera.h \
    lightrayrenderer.h \
    navigation.h \
    abstractgem.h \
    abstractgemrenderer.h \
    lightraydata.h \
    triangle.h \
    tetrahedrongem.h \
    cubegem.h \
    screenalignedquad.h \
    scenebounds.h \
    painter.h \
    painterqml.h \
    gemrenderernew.h \
    gemdata.h
