#include "abstractnavigation.h"

AbstractNavigation::AbstractNavigation(QObject *parent) :
    QObject(parent)
{

}

qreal AbstractNavigation::rotateX()
{
    return m_rotateX;
}

void AbstractNavigation::setRotateX(qreal rotateX)
{
    m_rotateX = rotateX;
}

qreal AbstractNavigation::rotateY()
{
    return m_rotateY;
}

void AbstractNavigation::setRotateY(qreal rotateY)
{
    m_rotateY = rotateY;
}

qreal AbstractNavigation::rotateZ()
{
    return m_rotateZ;
}

void AbstractNavigation::setRotateZ(qreal rotateZ)
{
    m_rotateZ = rotateZ;
}
