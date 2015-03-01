#include "abstractgem.h"

#include <limits>

#include <QQuaternion>
#include <QMatrix4x4>

#include "gemdata.h"
#include "lightray.h"
#include "triangle.h"

namespace {

float minimumWithLowerBound(float a, float b, float lowerBound)
{
    if (a > lowerBound) {
        return a <= b ? a : b;
    } else {
        return b > lowerBound ? b : std::numeric_limits<float>::max();
    }
}

}

AbstractGem::AbstractGem(QObject *parent) :
    QObject(parent)
  , m_data(new GemData())
  , m_radius(0.f)
{
}

AbstractGem::~AbstractGem()
{
    delete m_data;
}

void AbstractGem::setRotationFromEuler(const QVector3D &eulerRotation)
{
    QQuaternion newRotationX = QQuaternion::fromAxisAndAngle(QVector3D(1.f, 0.f, 0.f), eulerRotation.x());
    QQuaternion newRotationY = QQuaternion::fromAxisAndAngle(QVector3D(0.f, 1.f, 0.f), eulerRotation.y());
    QQuaternion newRotationZ = QQuaternion::fromAxisAndAngle(QVector3D(0.f, 0.f, 1.f), eulerRotation.z());
    QQuaternion newRoation = newRotationY * newRotationX * newRotationZ;
    setRotation(newRoation);
}

const QVector3D &AbstractGem::position() const
{
    return m_data->position();
}

void AbstractGem::setPosition(const QVector3D &position)
{
    if (position == m_data->position()) {
       return;
    }
    m_data->setPosition(position);
    emit positionChanged();
}

const QQuaternion &AbstractGem::rotation() const
{
    return m_data->rotation();
}

void AbstractGem::setRotation(const QQuaternion &rotation)
{
    if (rotation == m_data->rotation()) {
       return;
    }
    m_data->setRotation(rotation);
    emit rotationChanged();
}

qreal AbstractGem::scale() const
{
    return m_data->scale();
}

void AbstractGem::setScale(qreal scaleFactor)
{
    if (scaleFactor == m_data->scale()) {
       return;
    }
    m_data->setScale(scaleFactor);
    emit scaleChanged();
}

const QList<Triangle *> &AbstractGem::triangles() const
{
    return m_data->triangles();
}

GemType AbstractGem::type() const
{
    return m_data->type();
}

const QVector3D &AbstractGem::color() const
{
    return m_data->color();
}

void AbstractGem::setColor(const QVector3D &color)
{
    if (color == m_data->color()) {
        return;
    }
    m_data->setColor(color);
    emit colorChanged();
}

const GemData &AbstractGem::data() const
{
    return *m_data;
}

qreal AbstractGem::radius() const
{
    return m_radius * m_data->scale();
}

const QMatrix4x4 &AbstractGem::model() const
{
    return m_data->model();
}

float AbstractGem::boundingSphereIntersectedBy(const LightRay &ray, QVector3D *collisionPoint)
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

float AbstractGem::intersectedBy(const LightRay &ray, QVector3D *collisionPoint)
{
    Triangle *intersectedTriangle;
    return faceIntersectedBy(ray, intersectedTriangle, collisionPoint);
}

Triangle inWorldCoordinates(const Triangle &objectSpaceTriangle, const QMatrix4x4 & model)
{
    Triangle result;
    result.setA(model * objectSpaceTriangle.a());
    result.setB(model * objectSpaceTriangle.b());
    result.setC(model * objectSpaceTriangle.c());
    return result;
}

float AbstractGem::faceIntersectedBy(const LightRay &ray, Triangle *&intersectedTriangle, QVector3D *collisionPoint)
{
    const float maxFloat = std::numeric_limits<float>::max();
    const QVector3D noCollisionPoint(maxFloat, maxFloat, maxFloat);
    intersectedTriangle = nullptr;

    if (boundingSphereIntersectedBy(ray) == maxFloat) {
        if (collisionPoint) {
            *collisionPoint = noCollisionPoint;
        }
        return maxFloat;
    }

    // Calculate collision according to
    // http://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    // http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    const float epsilon = 0.0001f;
    float tPrevious = maxFloat;
    float t, u, v;
    QVector3D edge1, edge2, tvec, pvec, qvec;
    float det, invDet;

    for (auto objectSpaceTriangle : triangles()) {
        Triangle worldSpaceTriangle = inWorldCoordinates(*objectSpaceTriangle, model());

        edge1 = worldSpaceTriangle.b() - worldSpaceTriangle.a();
        edge2 = worldSpaceTriangle.c() - worldSpaceTriangle.a();

        // Calculate determinant
        pvec = QVector3D::crossProduct(ray.normalizedDirection(), edge2);
        det = QVector3D::dotProduct(edge1, pvec);

        // Non-culling branch
        if (det < -epsilon || det > epsilon) {
            invDet = 1.f / det;

            // Calculate distance from vert0 to ray origin
            tvec = ray.startPosition() - worldSpaceTriangle.a();

            // Calculate u and test bounds
            u = QVector3D::dotProduct(tvec, pvec) * invDet;
            if (u >= 0.f && u <= 1.f) {
                // Prepare to test v
                qvec = QVector3D::crossProduct(tvec, edge1);

                // Calculate v and test bounds
                v = QVector3D::dotProduct(ray.normalizedDirection(), qvec) * invDet;
                if ( v >= 0.f && u + v <= 1.0) {
                    // Calculate t, ray intersects triangle
                    t = QVector3D::dotProduct(edge2, qvec) * invDet;
                    //Use !qFuzzyIsNull(t) to ensure, that length of calculated vector is not treated as zero by qFuzzyIsEqual (required if calculated vectors should be normalized)
                    if (t < tPrevious && t > 0.0 + epsilon && !qFuzzyIsNull(t)) {
                        tPrevious = t;
                        delete intersectedTriangle;
                        intersectedTriangle = new Triangle(worldSpaceTriangle);
                        if (collisionPoint) {
                            *collisionPoint = ray.startPosition() + t * ray.normalizedDirection();
                        }
                    }
                }

            }
        }
    }
    return tPrevious;
}

void AbstractGem::rotate(const QQuaternion &quaternion)
{
    setRotation(quaternion * rotation());
}


uint qHash(GemType key, uint seed)
{
    return qHash(static_cast<uint>(key), seed);
}
