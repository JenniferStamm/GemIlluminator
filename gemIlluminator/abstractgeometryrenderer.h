#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>

class QOpenGLFunctions;

class AbstractGeometryRenderer : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGeometryRenderer(QObject *parent = 0);
    virtual ~AbstractGeometryRenderer();

    virtual void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection) = 0;

    QVector3D position();
    QVector3D rotation();
    QVector3D initialRotation() const;

    void setPosition(QVector3D position);
    void setRotation(QVector3D rotation);
    void setInitialRotation(const QVector3D &initialRotation);

protected:
    QVector3D m_position;
    QVector3D m_rotation;
    QVector3D m_initialRotation;
};

#endif // GEOMETRYRENDERER_H
