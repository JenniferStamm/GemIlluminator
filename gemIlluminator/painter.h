#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QSize;
class QTime;

class Camera;
class GlowEffect;
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

    void initializeEnvmap();

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
    void paintEnvmap(const Camera &camera);
    void renderLightRays(const Camera &camera);
    void renderScene(const Camera &camera);

protected:
    bool m_active;
    uint m_envmap;
    QOpenGLFunctions *m_gl;
    bool m_initialized;
    GlowEffect *m_glowEffect;
    uint m_glowFBO;
    uint m_glowDepthRB;
    uint m_glowTexture;
    uint m_previewSceneFBO;
    uint m_previewSceneDepthRB;
    uint m_previewSceneTexture;
    PainterQML *m_painterQML;
    ScreenAlignedQuad *m_quad;
    Scene *m_scene;
    uint m_sceneFBO;
    uint m_sceneDepthRB;
    uint m_sceneTexture;
    QMap<ShaderPrograms, QOpenGLShaderProgram*> *m_shaderPrograms;
    QSize *m_usedViewport;

    int m_counter;
    int m_oldElapsed;
    QTime *m_time;

};

#endif // PAINTER_H
