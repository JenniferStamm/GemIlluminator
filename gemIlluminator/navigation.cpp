#include "navigation.h"

Navigation::Navigation(QObject *parent) :
    QObject(parent)
{

}

qreal Navigation::rotateX()
{
    return m_rotateX;
}

void Navigation::setRotateX(qreal rotateX)
{
    m_rotateX = rotateX;
}

qreal Navigation::rotateY()
{
    return m_rotateY;
}

void Navigation::setRotateY(qreal rotateY)
{
    m_rotateY = rotateY;
}

qreal Navigation::rotateZ()
{
    return m_rotateZ;
}

void Navigation::setRotateZ(qreal rotateZ)
{
    m_rotateZ = rotateZ;
}
