#include "navigation.h"

#include <QtMath>
#include <QMatrix4x4>
#include <QQuaternion>

#include "camera.h"

Navigation::Navigation(QObject *parent) :
    QObject(parent)
  , m_camera(nullptr)
  , m_eulerRotation(new QVector3D())
{

}

Navigation::~Navigation()
{
    delete m_eulerRotation;
}

QQuaternion Navigation::rotation() const
{
    QQuaternion xRotation = QQuaternion::fromAxisAndAngle(QVector3D(1.f, 0.f, 0.f), m_eulerRotation->x());
    QQuaternion yRotation = QQuaternion::fromAxisAndAngle(QVector3D(0.f, 1.f, 0.f), m_eulerRotation->y());
    QQuaternion zRotation = QQuaternion::fromAxisAndAngle(QVector3D(0.f, 0.f, 1.f), m_eulerRotation->z());
    return fromEulerAngleQuaternions(xRotation, yRotation, zRotation);
}

QQuaternion Navigation::worldSpaceRotation() const
{
    if (!m_camera) {
        return rotation();
    }

    QVector3D xAxisView(1.f, 0.f, 0.f);
    QVector3D yAxisView(0.f, 1.f, 0.f);
    QVector3D zAxisView(0.f, 0.f, 1.f);

    QVector3D xAxisWorld = m_camera->view().transposed() * xAxisView;
    QVector3D yAxisWorld = m_camera->view().transposed() * yAxisView;
    QVector3D zAxisWorld = m_camera->view().transposed() * zAxisView;

    QQuaternion xRotation = QQuaternion::fromAxisAndAngle(xAxisWorld, m_eulerRotation->x());
    QQuaternion yRotation = QQuaternion::fromAxisAndAngle(yAxisWorld, m_eulerRotation->y());
    QQuaternion zRotation = QQuaternion::fromAxisAndAngle(zAxisWorld, m_eulerRotation->z());

    return fromEulerAngleQuaternions(xRotation, yRotation, zRotation);
}

void Navigation::setCamera(Camera *camera)
{
    m_camera = camera;
}

const QVector3D &Navigation::eulerRotation() const
{
    return *m_eulerRotation;
}

void Navigation::setEulerRotation(const QVector3D &angles)
{
    if (*m_eulerRotation == angles)
        return;

    *m_eulerRotation = angles;
    emit eulerRotationChanged();
}

QQuaternion Navigation::fromEulerAngleQuaternions(const QQuaternion &x, const QQuaternion &y, const QQuaternion &z) const
{
    return z * x * y;
}
