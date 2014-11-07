/*
 * Die Scene ist quasi die Schnittstelle zwischen QML und OpenGL.
 * Hier werden die nötigen Synchronisationen vorgenommen, die QML
 * verlangt. Die Scene ist ein QQuickItem, da dieses Zugriff auf
 * das Window erlaubt, wie auch im OpenGL-Beispiel.
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <QQuickItem>
#include <QQmlListProperty>

#include "cube.h"
#include "scenerenderer.h"

class Scene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(QQmlListProperty<Cube> cubes READ cubes)

public:
    explicit Scene(QQuickItem *parent = 0);
    virtual ~Scene();

    QQmlListProperty<Cube> cubes();
    int cubesCount() const;
    Cube* cubeAt(int index) const;

    qreal t();
    void setT(qreal t);

signals:
    void cubesChanged();
    void tChanged();

public slots:
    void sync();
    void cleanup();

protected:
    QList<Cube*> m_cubes;
    SceneRenderer *m_renderer;
    qreal m_t;

private slots:
    void handleWindowChanged(QQuickWindow *win);
};

#endif // RENDERER_H
