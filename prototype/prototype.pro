TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    squircle.cpp

RESOURCES += qml.qrc

QML_IMPORT_PATH =

include(deployment.pri)

HEADERS += \
    squircle.h

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml
