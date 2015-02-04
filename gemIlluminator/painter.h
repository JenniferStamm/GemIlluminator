#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QSize;

class Camera;
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
    QOpenGLFunctions * m_gl;
    bool m_initialized;
    PainterQML *m_painterQML;
    ScreenAlignedQuad *m_quad;
    Scene *m_scene;
    QMap<ShaderPrograms, QOpenGLShaderProgram*> *m_shaderPrograms;
    QSize *m_viewport;
};

#endif // PAINTER_H
