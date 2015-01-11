#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QMatrix4x4>
#include <QObject>
#include <QVector3D>

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

public:
    explicit AbstractGem(QObject *parent = 0);
    virtual ~AbstractGem();

    virtual void synchronize() = 0;
    virtual void cleanup() = 0;
    virtual void update(int timeDifference) = 0;

    void paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection, QOpenGLShaderProgram &program);

    QVector3D initialRotation() const;
    void setInitialRotation(const QVector3D &initialRotation);

    QVector3D position();
    void setPosition(QVector3D position);

    QVector3D rotation();
    void setRotation(QVector3D rotation);

    float rayIntersect(const LightRay &ray, QVector3D *collisionPoint = nullptr);
    virtual float rayIntersect(const LightRay &ray, int *triangleIndex, QVector3D *collisionPoint = nullptr) = 0;

signals:
    void initialRotationChanged();
    void positionChanged();
    void rotationChanged();

protected:
    int solveQuadricFormula(float a, float b, float c, float &x1, float &x2);

protected:
    AbstractGemRenderer *m_renderer;
    QVector3D m_initialRotation;
    QVector3D m_position;
    QVector3D m_rotation; /*!< Contains the euler angles */
};

#endif // GEOMETRY_H
