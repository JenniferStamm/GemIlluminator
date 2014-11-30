#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <QObject>
#include <QMatrix4x4>

class QOpenGLFunctions;

class AbstractGeometryRenderer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGeometryRenderer(QObject *parent = 0);
    virtual void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection) = 0;
};

#endif // GEOMETRYRENDERER_H
