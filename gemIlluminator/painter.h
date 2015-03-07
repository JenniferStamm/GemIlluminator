#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QSize;
class QTime;

class Camera;
class BlurEffect;
class CubeMap;
class PainterQML;
class ScreenAlignedQuad;
class Scene;
enum class ShaderPrograms;

/**
 * @brief The Painter class
 * @detail Includes the rendering process, thus creates the whole picture
 *
 */
class Painter : public QObject
{
    Q_OBJECT
public:
    Painter(PainterQML *painter, QObject *parent = 0);
    virtual ~Painter();

    void initializeEnvMap();

    bool isActive() const;
    void setActive(bool active);

    Scene *scene() const;
    void setScene(Scene *scene);

    QOpenGLFunctions &gl() const;

signals:

public slots:
    void paint();

protected:
    void initialize();
    void initializeFBOs();
    void initializeShaderPrograms();
    void renderLightRays(const Camera &camera);
    void renderScene(const Camera &camera);

protected:
    bool m_active;
    CubeMap *m_envMap;
    QOpenGLFunctions *m_gl;
    bool m_initialized;

    BlurEffect *m_blurEffectScene;
    int m_blurViewportRatioScene;
    uint m_glowSceneFBO;
    uint m_glowSceneDepthRB;
    uint m_glowSceneTexture;

    BlurEffect *m_blurEffectPreviewScene;
    int m_blurViewportRatioPreviewScene;
    uint m_glowPreviewSceneFBO;
    uint m_glowPreviewSceneDepthRB;
    uint m_glowPreviewSceneTexture;

    uint m_previewSceneFBO;
    uint m_previewSceneDepthRB;
    uint m_previewSceneTexture;

    PainterQML *m_painterQML;
    ScreenAlignedQuad *m_quad;

    CubeMap *m_rainbowMap;
    CubeMap *m_refractionMap;

    Scene *m_scene;

    uint m_sceneFBO;
    uint m_sceneDepthRB;
    uint m_sceneTexture;

    QHash<ShaderPrograms, QOpenGLShaderProgram*> *m_shaderPrograms;
    QSize *m_usedViewport;

    int m_counter;
    int m_oldElapsed;
    QTime *m_time;

};

#endif // PAINTER_H
