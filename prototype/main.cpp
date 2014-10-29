#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include "squircle.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Define a C++ object for use in qml
    qmlRegisterType<Squircle>("OpenGLUnderQML", 1, 0, "Squircle");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.show();


    return app.exec();
}
