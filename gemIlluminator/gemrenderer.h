#ifndef GEMRENDERER_H
#define GEMRENDERER_H

#include "abstractgemrenderer.h"

#include <QMatrix4x4>

#include "triangle.h"

class QOpenGLBuffer;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
template <typename T> class QVector;
class QVector3D;

class Triangle;

class GemRenderer : public AbstractGemRenderer
{
public:
    explicit GemRenderer(const QVector<Triangle *> &triangles, QObject *parent = 0);
    virtual ~GemRenderer();

    void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection, QOpenGLShaderProgram &program) override;

protected:
    virtual void initialize();
    virtual QVector<float>* initializeData(const QVector<Triangle *> &triangles);
    virtual void addTriangleData(
            Triangle &triangle,
            QVector<QVector3D> &data);

protected:
    bool m_initialized;
    QVector<float> *m_vertexData;
    QOpenGLBuffer *m_vertexBuffer;
};

#endif // GEMRENDERER_H
