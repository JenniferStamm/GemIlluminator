#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QSize;

class Camera;
class ScreenAlignedQuad;
class Scene;

/**
 * @brief The Painter class
 * @detail Includes the rendering process, thus creates the whole picture
 *
 */
class Painter : public QObject
{
    Q_OBJECT
public:
    Painter(QObject *parent = 0);
    virtual ~Painter();

    bool isActive() const;
    void setActive(bool active);

    void setCamera(const Camera &camera);

    Scene *scene() const;
    void setScene(Scene *scene);

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
    Scene *m_scene;
    QSize *m_viewport;
};

#endif // PAINTER_H
