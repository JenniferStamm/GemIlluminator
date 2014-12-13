#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include <QObject>
#include <QSize>
#include <QMatrix4x4>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class AbstractGem;

class SceneRenderer : public QObject
{
    Q_OBJECT
public:
    explicit SceneRenderer(QObject *parent = 0);
    void setViewport(QSize viewport);

    void setGeometries(QList<AbstractGem*> geometries);
    void setViewProjection(QMatrix4x4 viewProjection);

    bool isActive();
    void setActive(bool active);

public slots:
    void paint();

protected:
    void initialize();

protected:
    QSize m_viewport;
    QList<AbstractGem*> m_geometries;
    bool m_active;
    QOpenGLFunctions * m_gl;
    QMatrix4x4 m_viewProjection;
    QOpenGLShaderProgram *m_program;
};

#endif // SCENERENDERER_H
