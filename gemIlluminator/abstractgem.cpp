#include "abstractgem.h"

#include <limits>

#include "abstractgemrenderer.h"
#include "lightray.h"
#include "triangle.h"

AbstractGem::AbstractGem(QObject *parent) :
    QObject(parent)
  , m_renderer(nullptr)
  , m_initialRotation(new QVector3D())
  , m_position(new QVector3D())
  , m_rotation(new QVector3D())
  , m_scale(1.f)
  , m_radius(0.f)
{
}

AbstractGem::~AbstractGem()
{
    delete m_renderer;
    delete m_initialRotation;
    delete m_position;
    delete m_rotation;
}

void AbstractGem::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &program)
{
    if (m_renderer) {
        m_renderer->paint(gl, viewProjection, program);
    }
}

const QVector3D &AbstractGem::initialRotation() const
{
    return *m_initialRotation;
}

void AbstractGem::setInitialRotation(const QVector3D &initialRotation)
{
    if (initialRotation == *m_initialRotation) {
       return;
    }
    *m_initialRotation = initialRotation;
    emit initialRotationChanged();
}

const QVector3D &AbstractGem::position() const
{
    return *m_position;
}

void AbstractGem::setPosition(const QVector3D &position)
{
    if (position == *m_position) {
       return;
    }
    *m_position = position;
    emit positionChanged();
}

const QVector3D &AbstractGem::rotation() const
{
    return *m_rotation;
}

void AbstractGem::setRotation(const QVector3D &rotation)
{
    if (rotation == *m_rotation) {
       return;
    }
    *m_rotation = rotation;
    emit rotationChanged();
}

qreal AbstractGem::scale() const
{
    return m_scale;
}

void AbstractGem::setScale(qreal scaleFactor)
{
    if (scaleFactor == m_scale) {
       return;
    }
    m_scale = scaleFactor;
    emit scaleChanged();
}

qreal AbstractGem::radius() const
{
    return m_radius * m_scale;
}

int AbstractGem::solveQuadricFormula(float a, float b, float c, float &x1, float &x2)
{
    float p = b / a;
    float q = c / a;

    x1 = std::numeric_limits<float>::max();
    x2 = std::numeric_limits<float>::max();

    float discriminant = p * p * 0.25 - q;
    if (discriminant < 0) {
        return 0;
    } else {
        x1 = p * -0.5 - sqrt(discriminant);
        x2 = p * -0.5 + sqrt(discriminant);
        if (x1 == x2) {
            return 1;
        } else {
            return 2;
        }
    }
}

float minimumWithLowerBound(float a, float b, float lowerBound)
{
    if (a > lowerBound) {
        return a <= b ? a : b;
    } else {
        return b > lowerBound ? b : std::numeric_limits<float>::max();
    }
}

float AbstractGem::rayIntersect(const LightRay &ray, QVector3D *collisionPoint)
{
    const float maxFloat = std::numeric_limits<float>::max();
    const QVector3D noCollisionPoint(maxFloat, maxFloat, maxFloat);

    //|(ray.position + t * ray.direction - position)|^2 = radius^2
    float a;
    float b;
    float c;
    a = ray.direction().lengthSquared();
    QVector3D bVector = (ray.startPosition() - position()) * ray.direction();
    b = 2 * (bVector.x() + bVector.y() + bVector.z());
    c = (ray.startPosition() - position()).lengthSquared() - radius() * radius();

    float t1;
    float t2;
    int numberOfCollisions = solveQuadricFormula(a, b, c, t1, t2);
    if (numberOfCollisions == 0) {
        if (collisionPoint) {
            *collisionPoint = noCollisionPoint;
        }
        return maxFloat;
    } else {
        float t = minimumWithLowerBound(t1, t2, 0.01f);
        if (collisionPoint) {
            if (t != maxFloat) {
                *collisionPoint = ray.startPosition() + t * ray.direction();
            } else {
                *collisionPoint = noCollisionPoint;
            }
        }
        return t;
    }
}
