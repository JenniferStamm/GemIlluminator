TEMPLATE = app

QT += qml quick widgets sensors opengl openglextensions #multimedia

SOURCES += main.cpp \
    scene.cpp \
    scenerenderer.cpp \
    player.cpp \
    lightray.cpp \
    camera.cpp \
    lightrayrenderer.cpp \
    navigation.cpp \
    abstractgem.cpp \
    lightraydata.cpp \
    triangle.cpp \
    tetrahedrongem.cpp \
    cubegem.cpp \
    screenalignedquad.cpp \
    scenebounds.cpp \
    config.cpp \
    painter.cpp \
    painterqml.cpp \
    gemdata.cpp \
    gemrenderer.cpp \
    gloweffect.cpp

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
    camera.h \
    lightrayrenderer.h \
    navigation.h \
    abstractgem.h \
    lightraydata.h \
    triangle.h \
    tetrahedrongem.h \
    cubegem.h \
    screenalignedquad.h \
    scenebounds.h \
    config.h \
    painter.h \
    painterqml.h \
    gemdata.h \
    gemrenderer.h \
    gloweffect.h

release: DESTDIR = $$OUT_PWD/release
debug:   DESTDIR = $$OUT_PWD/debug


win32 {
    install_it.path = $$DESTDIR/assets
    install_it.files = assets/config.json

    INSTALLS += install_it
}
