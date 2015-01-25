#ifndef GEOMETRYRENDERER_H
#define GEOMETRYRENDERER_H

#include <QMatrix4x4>
#include <QObject>
#include <QVector3D>

class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QQuaternion;
class QVector3D;

class AbstractGemRenderer : public QObject
{
    Q_OBJECT

public:
    explicit AbstractGemRenderer(QObject *parent = 0);
    virtual ~AbstractGemRenderer();

    virtual void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program) = 0;

    const QQuaternion &initialRotation() const;
    void setInitialRotation(const QQuaternion &initialRotation);

    const QVector3D &position() const;
    void setPosition(const QVector3D &position);

    const QQuaternion &rotation() const;
    void setRotation(const QQuaternion &rotation);

    qreal scale() const;
    void setScale(qreal scaleFactor);

    const QVector3D &color() const;
    void setColor(const QVector3D &color);

protected:
    QQuaternion *m_initialRotation;
    QVector3D *m_position;
    QQuaternion *m_rotation;
    qreal m_scale;
    QVector3D *m_color;
};

#endif // GEOMETRYRENDERER_H
