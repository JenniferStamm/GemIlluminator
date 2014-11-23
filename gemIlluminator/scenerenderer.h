#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include <QObject>
#include <QSize>

class QOpenGLFunctions;
class AbstractGeometry;

class SceneRenderer : public QObject
{
    Q_OBJECT
public:
    explicit SceneRenderer(QObject *parent = 0);
    void setViewport(QSize viewport);

    void setGeometries(QList<AbstractGeometry*> geometries);

    bool isActive();
    void setActive(bool active);

public slots:
    void paint();

protected:
    QSize m_viewport;
    QList<AbstractGeometry*> m_geometries;
    bool m_active;
    QOpenGLFunctions * m_gl;
};

#endif // SCENERENDERER_H