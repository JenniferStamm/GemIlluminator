#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include <QMap>
#include <QObject>
#include <QOpenGLShaderProgram>

#include "scene.h"

class QOpenGLFunctions;
class QMatrix4x4;

class AbstractGem;
class GemRenderer;
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

    void cleanup(QOpenGLFunctions &gl);

    void synchronizeGeometries(QList<AbstractGem*> geometries);

    LightRay *rootLightRay() const;
    void setRootLightRay(LightRay *rootLightRay);

public slots:
    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QMap<ShaderPrograms, QOpenGLShaderProgram*> &shaderPrograms);

protected:
    void paintGems(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram);
    void paintLightRays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram);

protected:
    GemRenderer *m_gemRenderer;
    QList<AbstractGem*> m_geometries;
    LightRay *m_rootLightRay;
};

#endif // SCENERENDERER_H
