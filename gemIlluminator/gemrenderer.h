#ifndef GEMRENDERER_H
#define GEMRENDERER_H

#include "abstractgemrenderer.h"
#include <QMatrix4x4>

template <typename T> class QVector;
class QVector3D;
class QOpenGLFunctions;
class QOpenGLBuffer;
class QOpenGLShaderProgram;

class GemRenderer : public AbstractGemRenderer
{
public:
    explicit GemRenderer(QVector<QVector3D> *vertices, QVector<QVector3D> *colors, QObject *parent = 0);
    virtual ~GemRenderer();

    void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection, QOpenGLShaderProgram &program) override;

protected:
    virtual void initialize();
    virtual QVector<float>* initializeData(
            QVector3D vector1,
            QVector3D vector2,
            QVector3D vector3,
            QVector3D vector4,
            QVector3D color1,
            QVector3D color2,
            QVector3D color3,
            QVector3D color4);
    virtual void addTriangleData(
            QVector3D vector1,
            QVector3D vector2,
            QVector3D vector3,
            QVector3D color1,
            QVector<QVector3D>* data);
    virtual QVector3D calculateNormal(
            QVector3D vector1,
            QVector3D vector2,
            QVector3D vector3);

protected:
    bool m_initialized;
    QVector<float> *m_vertexData;
    QOpenGLBuffer *m_vertexBuffer;
};

#endif // GEMRENDERER_H
