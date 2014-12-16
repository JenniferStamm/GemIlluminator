#ifndef LIGHTRAYRENDERER_H
#define LIGHTRAYRENDERER_H

#include <QObject>

class Camera;
class LightRay;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
template<typename T> class QVector;

class LightRayRenderer : public QObject
{
    Q_OBJECT

public:
    explicit LightRayRenderer(QObject *parent = 0);
    virtual ~LightRayRenderer();

    void setCamera(Camera & camera);

    void addLightRay(const LightRay & lightRay);

    virtual void paint(QOpenGLFunctions *gl);

signals:

public slots:

protected:
    bool m_isInitialized;
    QOpenGLShaderProgram *m_program;
    Camera *m_camera;
    QVector<LightRay *> *m_rays;
};

#endif // LIGHTRAYRENDERER_H
