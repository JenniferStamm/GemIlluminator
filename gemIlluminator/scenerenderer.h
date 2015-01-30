#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QMatrix4x4;


class AbstractGem;
class LightRay;

/**
 * @brief The SceneRenderer class
 * @detail Renders the scene: Packs the scene in the buffer
 * and draws the scene in one call
 */
class SceneRenderer : public QObject
{
    Q_OBJECT

public:
    explicit SceneRenderer(QObject *parent = 0);
    virtual ~SceneRenderer();

    void setGeometries(QList<AbstractGem*> geometries);

public slots:
    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &gemProgram);

protected:
    QList<AbstractGem*> m_geometries;
};

#endif // SCENERENDERER_H
