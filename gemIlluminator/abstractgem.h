#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>
#include <QVector3D>

class QMatrix4x4;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

class AbstractGemRenderer;
class LightRay;
class Triangle;

class AbstractGem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVector3D initialRotation READ initialRotation WRITE setInitialRotation NOTIFY initialRotationChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)

public:
    explicit AbstractGem(QObject *parent = 0);
    virtual ~AbstractGem();

    virtual void synchronize() = 0;
    virtual void cleanup() = 0;
    virtual void update(int timeDifference) = 0;

    void paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program);

    const QVector3D &initialRotation() const;
    void setInitialRotation(const QVector3D &initialRotation);

    const QVector3D &position() const;
    void setPosition(const QVector3D &position);

    const QVector3D &rotation() const;
    void setRotation(const QVector3D &rotation);

    qreal scale() const;
    void setScale(qreal scaleFactor);

    qreal radius() const;

    float rayIntersect(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    virtual float rayIntersect(const LightRay &ray, int &triangleIndex, QVector3D *collisionPoint = nullptr) = 0;

signals:
    void initialRotationChanged();
    void positionChanged();
    void rotationChanged();
    void scaleChanged();

protected:
    int solveQuadricFormula(float a, float b, float c, float &x1, float &x2);

protected:
    AbstractGemRenderer *m_renderer;
    QVector3D *m_initialRotation;
    QVector3D *m_position;
    QVector3D *m_rotation; /*!< Contains the euler angles */
    qreal m_scale;
    qreal m_radius;
};

#endif // GEOMETRY_H
