#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QSize;

class Camera;
class LightRay;
class ScreenAlignedQuad;
class SceneRenderer;

/**
 * @brief The Painter class
 * @detail Includes the rendering process, thus creates the whole picture
 *
 */
class Painter : public QObject
{
    Q_OBJECT
public:
    Painter(SceneRenderer &sceneRenderer, QObject *parent = 0);
    virtual ~Painter();

    void setCamera(const Camera &camera);

    bool isActive() const;
    void setActive(bool active);

    SceneRenderer &sceneRenderer() const;

    LightRay *rootLightRay() const;
    void setRootLightRay(LightRay *rootLightRay);

    void setViewport(const QSize &viewport);

signals:

public slots:
    void paint();

protected:
    void initialize();
    void initializeEnvmap();
    void paintEnvmap();

protected:
    bool m_active;
    Camera *m_camera;
    uint m_envmap;
    QOpenGLShaderProgram *m_envmapProgram;
    QOpenGLShaderProgram *m_gemProgram;
    QOpenGLFunctions * m_gl;
    bool m_initialized;
    ScreenAlignedQuad *m_quad;
    LightRay *m_rootLightRay;
    SceneRenderer &m_sceneRenderer;
    QSize *m_viewport;
};

#endif // PAINTER_H
