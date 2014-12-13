#ifndef LIGHTRAYRENDERER_H
#define LIGHTRAYRENDERER_H

#include <QObject>

class Camera;
class LightRay;
class QOpenGLProgram;
class QOpenGLFunctions;

class LightRayRenderer : public QObject
{
    Q_OBJECT
public:
    explicit LightRayRenderer(QObject *parent = 0);
    virtual ~LightRayRenderer();

    void setCamera(const Camera & camera);
    void addLightRay(const LightRay & ray);

    virtual void paint(QOpenGLFunctions *gl);

signals:

public slots:

protected:
    bool m_isInitialized;
    QOpenGLProgram *m_program;
    Camera *m_camera;
};

#endif // LIGHTRAYRENDERER_H
