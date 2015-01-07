#ifndef GEMRENDERER_H
#define GEMRENDERER_H

#include "abstractgemrenderer.h"
#include <QMatrix4x4>

template <typename T> class QVector;
class QVector3D;
class QOpenGLFunctions;
class QOpenGLBuffer;
class QOpenGLShaderProgram;
class Triangle;

class GemRenderer : public AbstractGemRenderer
{
public:
    explicit GemRenderer(Triangle *triangles[4], QObject *parent = 0);
    virtual ~GemRenderer();

    void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection, QOpenGLShaderProgram &program) override;

protected:
    virtual void initialize();
    virtual QVector<float>* initializeData(Triangle *triangles[4]);
    virtual void addTriangleData(
            Triangle *triangle,
            QVector<QVector3D>* data);

protected:
    bool m_initialized;
    QVector<float> *m_vertexData;
    QOpenGLBuffer *m_vertexBuffer;
};

#endif // GEMRENDERER_H
