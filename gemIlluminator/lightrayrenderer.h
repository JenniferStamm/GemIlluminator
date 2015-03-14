#ifndef LIGHTRAYRENDERER_H
#define LIGHTRAYRENDERER_H

#include <QObject>

class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
template<typename T> class QSet;
template<typename T> class QVector;

class LightRay;
class LightRayData;

class LightRayRenderer : public QObject
{
    Q_OBJECT
public:
    explicit LightRayRenderer(QObject *parent = 0);
    virtual ~LightRayRenderer();

    void addLightRay(const LightRay &lightRay);
    virtual void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram);
    void resetDynamicRays();

protected:
    void calculateVertexDataFor(const LightRayData & rayData, QVector<float> &vertices, QVector<unsigned int> & indices);
    void updateDynamicVBO();
    void updateStaticVBO();
    void updateRayVBO(QOpenGLBuffer *&vertexBuffer, QOpenGLBuffer *&indexBuffer, const QSet<LightRayData> &data);

protected:
    QOpenGLBuffer *m_dynamicIndexBuffer;
    QSet<LightRayData> *m_dynamicRays;
    QOpenGLBuffer *m_dynamicVertexBuffer;
    bool m_isStaticVBOUpdateRequired;
    QOpenGLBuffer *m_staticIndexBuffer;
    QSet<LightRayData> *m_staticRays;
    QOpenGLBuffer *m_staticVertexBuffer;
};

#endif // LIGHTRAYRENDERER_H
