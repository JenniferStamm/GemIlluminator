#ifndef LIGHTRAYRENDERER_H
#define LIGHTRAYRENDERER_H

#include <QObject>

class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
template<typename T> class QSet;
template<typename T> class QVector;

class Camera;
class LightRay;
class LightRayData;

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
    void initialize();
    void updateStaticVBO();

protected:
    bool m_isInitialized;
    bool m_isStaticVBOUpdateRequired;
    QOpenGLShaderProgram *m_program;
    QOpenGLBuffer *m_vertexBuffer;
    QOpenGLBuffer *m_indexBuffer;
    Camera *m_camera;
    QVector<LightRay *> *m_dynamicRays;
    QSet<LightRayData> *m_staticRays;
};

#endif // LIGHTRAYRENDERER_H
