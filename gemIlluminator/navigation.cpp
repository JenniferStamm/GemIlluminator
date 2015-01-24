#include "navigation.h"

Navigation::Navigation(QObject *parent) :
    QObject(parent)
{

}

QQuaternion Navigation::rotateX()
{
    return m_rotateX;
}

void Navigation::setRotateX(QQuaternion rotateX)
{
    m_rotateX = rotateX;
}

QQuaternion Navigation::rotateY()
{
    return m_rotateY;
}

void Navigation::setRotateY(QQuaternion rotateY)
{
    m_rotateY = rotateY;
}

QQuaternion Navigation::rotateZ()
{
    return m_rotateZ;
}

void Navigation::setRotateZ(QQuaternion rotateZ)
{
    m_rotateZ = rotateZ;
}
