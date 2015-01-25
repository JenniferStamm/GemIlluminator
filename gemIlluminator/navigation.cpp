#include "navigation.h"

Navigation::Navigation(QObject *parent) :
    QObject(parent)
{

}

QQuaternion Navigation::rotation()
{
    return m_rotation;
}

void Navigation::setRotation(QQuaternion rotation)
{
    if (m_rotation == rotation) {
        return;
    }
    m_rotation = rotation;
    emit rotationChanged();
}

void Navigation::setRotationFromEuler(const QVector3D &eulerAngles)
{
    QQuaternion newRotationX = QQuaternion::fromAxisAndAngle(QVector3D(1.f, 0.f, 0.f), eulerAngles.x());
    QQuaternion newRotationY = QQuaternion::fromAxisAndAngle(QVector3D(0.f, 1.f, 0.f), eulerAngles.y());
    QQuaternion newRotationZ = QQuaternion::fromAxisAndAngle(QVector3D(0.f, 0.f, 1.f), eulerAngles.z());
    QQuaternion newRoation = newRotationZ * newRotationX * newRotationY;
    setRotation(newRoation);
}
