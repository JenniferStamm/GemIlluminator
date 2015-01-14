#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QMatrix4x4;
class QSize;

class AbstractGem;
class LightRay;
class ScreenAlignedQuad;

class SceneRenderer : public QObject
{
    Q_OBJECT

public:
    explicit SceneRenderer(QObject *parent = 0);
    virtual ~SceneRenderer();

    void setViewport(const QSize &viewport);

    void setGeometries(QList<AbstractGem*> geometries);
    void setProjectionInverted(const QMatrix4x4 &projectionInverted);
    void setView(const QMatrix4x4 &view);
    void setViewProjection(const QMatrix4x4 &viewProjection);

    bool isActive() const;
    void setActive(bool active);

    LightRay *rootLightRay() const;
    void setRootLightRay(LightRay *rootLightRay);

public slots:
    void paint();

protected:
    void initialize();

protected:
    bool m_initialized;
    QSize *m_viewport;
    QList<AbstractGem*> m_geometries;
    bool m_active;
    QOpenGLFunctions * m_gl;
    QMatrix4x4 *m_projectionInverted;
    QMatrix4x4 *m_view;
    QMatrix4x4 *m_viewProjection;
    QOpenGLShaderProgram *m_gemProgram;
    QOpenGLShaderProgram *m_lightProgram;
    LightRay *m_rootLightRay;

    uint m_envmap;
    QOpenGLShaderProgram *m_envmapProgram;
    ScreenAlignedQuad *m_quad;
};

#endif // SCENERENDERER_H
