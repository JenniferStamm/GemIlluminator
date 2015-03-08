#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include <QObject>
#include <QOpenGLShaderProgram>

#include "scene.h"

class QOpenGLFunctions;
class QMatrix4x4;

class AbstractGem;
class GemRenderer;
class LightRay;
class LightRayRenderer;

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
    void synchronizeLightRays(LightRay *rootLightRay);

    void paintGems(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram);
    void paintLightRays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram);
    void paintLightRays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QHash<ShaderPrograms, QOpenGLShaderProgram*> &shaderProgram);

public slots:
    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QHash<ShaderPrograms, QOpenGLShaderProgram*> &shaderPrograms);

signals:
    void initalizationDone();

protected:
    void initalize(QOpenGLFunctions &gl);

protected:
    GemRenderer *m_gemRenderer;
    LightRayRenderer *m_lightRayRenderer;
    bool m_isInitalized;
};

#endif // SCENERENDERER_H
