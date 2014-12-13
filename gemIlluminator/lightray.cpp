#include "lightray.h"

#include "player.h"

LightRay::LightRay(QObject *parent) :
    AbstractGeometry(parent)
  , m_startPosition(new QVector3D())
  , m_endPosition(new QVector3D())
  , m_direction(new QVector3D())
{
}

LightRay::~LightRay()
{
    delete m_startPosition;
    delete m_endPosition;
    delete m_direction;
}

void LightRay::synchronize()
{

}

void LightRay::cleanup()
{

}

void LightRay::update(int timeDifference)
{
    if (m_player){
        QVector3D playerPosition = m_player->position();
        playerPosition += (direction() * m_player->velocity() * timeDifference) / 1000;
        m_player->setPosition(playerPosition);
    }
}

const QVector3D & LightRay::startPosition()
{
    return *m_startPosition;
}

void LightRay::setStartPosition(const QVector3D &position)
{
    if (*m_startPosition == position){
        return;
    }
    *m_startPosition = position;
    *m_direction = *m_endPosition - *m_startPosition;
    emit startPositionChanged();
}

const QVector3D & LightRay::endPosition()
{
    return *m_endPosition;
}

void LightRay::setEndPosition(const QVector3D &position)
{
    if (*m_endPosition == position){
        return;
    }
    *m_endPosition = position;
    *m_direction = *m_endPosition - *m_startPosition;
    emit endPositionChanged();
}

const QVector3D & LightRay::direction()
{
    return *m_direction;
}
