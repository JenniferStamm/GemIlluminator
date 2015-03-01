#ifndef GLOWEFFECT_H
#define GLOWEFFECT_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QSize;

class Camera;
class ScreenAlignedQuad;
enum class ShaderPrograms;

class BlurEffect : public QObject
{
    Q_OBJECT
public:
    BlurEffect(QOpenGLFunctions &gl, uint glowTexture, QObject *parent = nullptr);
    virtual ~BlurEffect();

    void renderGlowToTexture(const Camera &camera);

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

    uint m_blurFBO;
    uint m_blurDepthRB;
    uint m_blurTexture;
    uint m_secondaryBlurFBO;
    uint m_secondaryBlurTexture;

    QSize *m_usedViewport;

    ScreenAlignedQuad *m_quad;
};

#endif // GLOWEFFECT_H
