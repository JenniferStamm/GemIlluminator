#ifndef PAINTER_H
#define PAINTER_H

#include <QObject>
#include <QOpenGLFunctions>

class QOpenGLShaderProgram;
class QSize;

class Camera;
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
    Painter(QObject *parent = 0);
    virtual ~Painter();

    void initializeEnvmap();

    bool isActive() const;
    void setActive(bool active);

    void setCamera(const Camera &camera);

    Scene *scene() const;
    void setScene(Scene *scene);

    void setViewport(const QSize &viewport);

    QString envMapPrefix() const;
    void setEnvMapPrefix(const QString &envMapPrefix);

signals:

public slots:
    void paint();

protected:
    void initialize();
    void initializeBuffers();
    void initializeShaderPrograms();
    void initializeTextures();
    void paintEnvmap(const Camera &camera);
    void renderPreviewScene();
    void renderScene();

protected:
    bool m_active;
    Camera *m_camera;
    uint m_envmap;
    QString m_envMapPrefix;
    GLuint m_fbo;
    QOpenGLFunctions * m_gl;
    bool m_initialized;
    GLuint m_previewSceneDepthRB;
    GLuint m_previewSceneTexture;
    ScreenAlignedQuad *m_quad;
    Scene *m_scene;
    GLuint m_sceneDepthRB;
    GLuint m_sceneTexture;
    QMap<ShaderPrograms, QOpenGLShaderProgram*> *m_shaderPrograms;
    QSize *m_viewport;
};

#endif // PAINTER_H
