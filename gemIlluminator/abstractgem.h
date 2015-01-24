#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>
#include <QQuaternion>
#include <QVector3D>

class QMatrix4x4;
class QOpenGLFunctions;
class QOpenGLShaderProgram;
class QQuaternion;

class AbstractGemRenderer;
class LightRay;
class Triangle;

class AbstractGem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D initialRotation READ initialRotation WRITE setInitialRotation NOTIFY initialRotationChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QVector3D color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit AbstractGem(QObject *parent = 0);
    virtual ~AbstractGem();

    virtual void synchronize() = 0;
    virtual void cleanup() = 0;
    virtual void update(int timeDifference) = 0;

    virtual void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);

    const QVector3D &initialRotation() const;
    virtual void setInitialRotation(const QVector3D &initialRotation);

    const QVector3D &position() const;
    virtual void setPosition(const QVector3D &position);

    const QQuaternion &rotation() const;
    virtual void setRotation(const QQuaternion &rotation);

    qreal scale() const;
    void setScale(qreal scaleFactor);

    QVector3D &color() const;
    void setColor(QVector3D &color);

    qreal radius() const;

    float boundingSphereIntersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    float intersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    float faceIntersectedBy(const LightRay &ray, Triangle *&intersectedFace, QVector3D *collisionPoint = nullptr);

signals:
    void initialRotationChanged();
    void positionChanged();
    void rotationChanged();
    void scaleChanged();
    void colorChanged();

protected:
    int solveQuadricFormula(float a, float b, float c, float &x1, float &x2);

protected:
    QVector<Triangle*> *m_triangles;
    QVector3D *m_color;
    AbstractGemRenderer *m_renderer;
    QVector3D *m_initialRotation;
    QVector3D *m_position;
    QQuaternion *m_rotation;
    qreal m_scale;
    qreal m_radius;
};

#endif // GEOMETRY_H
