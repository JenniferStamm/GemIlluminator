#ifndef GLOWEFFECT_H
#define GLOWEFFECT_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QSize;

class Camera;
class ScreenAlignedQuad;
enum class ShaderPrograms;

class GlowEffect : public QObject
{
    Q_OBJECT
public:
    GlowEffect(QOpenGLFunctions &gl, const Camera &camera, QObject *parent = nullptr);
    ~GlowEffect();

    void renderGlowToTexture(uint glowTexture);

protected:
    void initialize();
    void initializeFBOs();
    void initializeShaderPrograms();
    void renderGaussHorizontal(const Camera &camera, int viewportWidth);
    void renderGaussVertical(const Camera &camera, int viewportHeight);

protected:
    const Camera &m_camera;
    QOpenGLFunctions &m_gl;
    QMap<ShaderPrograms, QOpenGLShaderProgram*> *m_shaderPrograms;
    bool m_initialized;

    uint m_lightRayFBO;
    uint m_lightRayDepthRB;
    uint m_lightRayTexture;
    uint m_secondaryLightRayFBO;
    uint m_secondaryLightRayTexture;

    QSize *m_usedViewport;

    ScreenAlignedQuad *m_quad;
};

#endif // GLOWEFFECT_H
