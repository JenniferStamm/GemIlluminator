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
class GemData;
class LightRay;
class Triangle;

namespace Gem {

enum class Type {
    Abstract,
    Cube,
    Tetrahedron};
}

class AbstractGem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QVector3D &position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(const QQuaternion &rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(const QVector3D &color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit AbstractGem(QObject *parent = 0);
    virtual ~AbstractGem();

    virtual void synchronize() = 0;
    virtual void cleanup() = 0;

    virtual void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);

    const QVector3D &color() const;
    void setColor(QVector3D &color);

    const GemData &data() const;

    const QMatrix4x4 &model() const;

    const QVector3D &position() const;
    virtual void setPosition(const QVector3D &position);

    qreal radius() const;

    const QQuaternion &rotation() const;
    virtual void setRotation(const QQuaternion &rotation);

    qreal scale() const;
    void setScale(qreal scaleFactor);

    const QList<Triangle *> &triangles() const;
    Gem::Type type() const;

    float boundingSphereIntersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    float intersectedBy(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    float faceIntersectedBy(const LightRay &ray, Triangle *&intersectedFace, QVector3D *collisionPoint = nullptr);
    void rotate(const QQuaternion &quaternion);

public slots:
    void setRotationFromEuler(const QVector3D &eulerRotation);

signals:
    void positionChanged();
    void rotationChanged();
    void scaleChanged();
    void colorChanged();

protected:
    int solveQuadricFormula(float a, float b, float c, float &x1, float &x2);

protected:
    GemData *m_data;
    AbstractGemRenderer *m_renderer;
    qreal m_radius;
};

#endif // GEOMETRY_H
