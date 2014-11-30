#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <QObject>
#include <QVector3D>

class QOpenGLFunctions;

class AbstractGeometryRenderer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGeometryRenderer(QObject *parent = 0);
    virtual ~AbstractGeometryRenderer();

    virtual void paint(QOpenGLFunctions *gl) = 0;

    QVector3D position();
    QVector3D rotation();

    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);

protected:
    QVector3D m_position;
    QVector3D m_rotation;
};

#endif // GEOMETRYRENDERER_H
