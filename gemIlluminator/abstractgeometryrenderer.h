#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <QObject>

class QOpenGLFunctions;

class AbstractGeometryRenderer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGeometryRenderer(QObject *parent = 0);
    virtual void paint(QOpenGLFunctions *gl) = 0;
};

#endif // GEOMETRYRENDERER_H
