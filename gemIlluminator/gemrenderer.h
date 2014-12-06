#ifndef GEMRENDERER_H
#define GEMRENDERER_H

#include "abstractgeometryrenderer.h"
#include <QMatrix4x4>

template <typename T> class QVector;
class QVector3D;
class QOpenGLFunctions;
class QOpenGLBuffer;

class GemRenderer : public AbstractGeometryRenderer
{
public:
    explicit GemRenderer(QVector<QVector3D> *vertices, QVector<QVector3D> *colors, QObject *parent = 0);
    virtual ~GemRenderer();

    void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection) override;

protected:
    virtual void initialize();
    virtual QVector<QVector3D>* initializeVertexData(
            QVector3D vector1,
            QVector3D vector2,
            QVector3D vector3,
            QVector3D vector4,
            QVector3D color1,
            QVector3D color2,
            QVector3D color3,
            QVector3D color4);
    virtual QVector3D calculateNormal(
            QVector3D vector1,
            QVector3D vector2,
            QVector3D vector3);

protected:
    QVector<QVector3D> *m_vertexData;
    QOpenGLBuffer *m_vertexBuffer;
};

#endif // GEMRENDERER_H
