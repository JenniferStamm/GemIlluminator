#ifndef GEMRENDERER_H
#define GEMRENDERER_H

#include "abstractgemrenderer.h"

class QMatrix4x4;
class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
template <typename T> class QVector;
class QVector3D;

class Triangle;

class GemRenderer : public AbstractGemRenderer
{
public:
    explicit GemRenderer(const QList<Triangle *> &triangles, QObject *parent = 0);
    virtual ~GemRenderer();

    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program) override;

protected:
    virtual void initialize();
    virtual QVector<float>* initializeData(const QList<Triangle *> &triangles);
    virtual void addTriangleData(
            Triangle &triangle,
            QVector<QVector3D> &data);

protected:
    bool m_initialized;
    QVector<float> *m_vertexData;
    QOpenGLBuffer *m_vertexBuffer;
};

#endif // GEMRENDERER_H
