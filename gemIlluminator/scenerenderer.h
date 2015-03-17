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
enum class ShaderPrograms;

/**
 * @brief The SceneRenderer class
 * @detail Renders the scene: Packs the scene in the buffer
 * and draws the scene in as few render calls as possible. The SceneRenderer uses specialized Renderer for diffrent types of geometry.
 */
class SceneRenderer : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief SceneRenderer
     * @param parent
     */
    explicit SceneRenderer(QObject *parent = 0);
    virtual ~SceneRenderer();

    /**
     * @brief Clears all used resources.
     * @param gl QOpenGLFunctions which can be used by SceneRenderer to release GPU resources
     */
    void cleanup(QOpenGLFunctions &gl);

    /**
     * @brief Synchronizes given gems between gui and render thread.
     * @param geometries List of gems which should be synchronized.
     */
    void synchronizeGeometries(QList<AbstractGem*> geometries);

    /**
     * @brief Synchronizes light rays between gui and render thread.
     * @param rootLightRay This and all of its successors will be synchronized.
     */
    void synchronizeLightRays(LightRay *rootLightRay);

    /**
     * @brief Paints the previous synchronized gems using specified program.
     * @param gl QOpenGLFunctions which will be used for any gl-call
     * @param viewProjection The viewprojection matrix which will be used.
     * @param shaderProgram The specified shaderprogram used for rendering.
     */
    void paintGems(QOpenGLFunctions &gl, QOpenGLShaderProgram &shaderProgram);
    /**
     * @brief Paints previous synchronized light rays using specified program.
     * @param gl QOpenGLFunctions which will be used for any gl-call
     * @param viewProjection The viewprojection matrix which will be used.
     * @param shaderProgram The specified shaderprogram used for rendering.
     */
    void paintLightRays(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram);

public slots:
    /**
     * @brief paint Paints the previous synchronized scene using specified programs.
     * @param gl QOpenGLFunctions which will be used for any gl-call
     * @param viewProjection The viewprojection matrix which will be used.
     * @param shaderPrograms A QHash containing different shader programs for different components of scene (lightrays and gems)
     */
    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, const QHash<ShaderPrograms, QOpenGLShaderProgram*> &shaderPrograms);

signals:
    /**
     * @brief This signal is emitted after initialization of all resources is done.
     */
    void initalizationDone();

protected:
    void initalize(QOpenGLFunctions &gl);

protected:
    GemRenderer *m_gemRenderer;
    LightRayRenderer *m_lightRayRenderer;
    bool m_isInitalized;
};

#endif // SCENERENDERER_H
