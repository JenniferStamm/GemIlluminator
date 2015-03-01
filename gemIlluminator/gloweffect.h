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
    GlowEffect(QOpenGLFunctions &gl, QObject *parent = nullptr);
    virtual ~GlowEffect();

    void renderGlowToTexture( const Camera &camera, uint glowTexture);

protected:
    void initialize();
    void initializeFBOs();
    void initializeShaderPrograms();
    void renderGaussHorizontal(const Camera &camera, int viewportWidth);
    void renderGaussVertical(const Camera &camera, int viewportHeight);

protected:
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
