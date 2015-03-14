#ifndef GLOWEFFECT_H
#define GLOWEFFECT_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QSize;

class Camera;
class ScreenAlignedQuad;
enum class ShaderPrograms;

/**
 * @brief The BlurEffect blurs a given texture.
 */
class BlurEffect : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Creates a new BlurEffect, that will blur a specified texture everytime blur() is called
     * @param gl The QOpenGLFuntions will be used for all gl-calls.
     * @param glowTexture The texture that will be blurred.
     * @param parent QObject-parent
     */
    BlurEffect(QOpenGLFunctions &gl, uint glowTexture, QObject *parent = nullptr);
    virtual ~BlurEffect();

    void blur(const QSize &textureSize);

protected:
    void initialize();
    void initializeFBOs();
    void initializeShaderPrograms();
    void renderGaussHorizontal(const QSize &textureSize);
    void renderGaussVertical(const QSize &textureSize);

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
