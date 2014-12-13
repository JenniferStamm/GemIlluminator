#include "lightray.h"

#include "lightrayrenderer.h"
#include "player.h"


LightRay::LightRay(QObject *parent) :
    QObject(parent)
  , m_player(nullptr)
  , m_renderer(nullptr)
  , m_successors(new QList<LightRay*>())
  , m_startPosition(new QVector3D())
  , m_endPosition(new QVector3D())
  , m_direction(new QVector3D())
{
}

LightRay::LightRay(LightRay &lightRay, QObject *parent) :
    QObject(parent)
  , m_player(nullptr)
  , m_renderer(nullptr)
  , m_successors(new QList<LightRay*>())
  , m_startPosition(new QVector3D(lightRay.startPosition()))
  , m_endPosition(new QVector3D(lightRay.endPosition()))
  , m_direction(new QVector3D(lightRay.direction()))
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
    if (!m_renderer) {
        m_renderer = new LightRayRenderer();
    }

    m_renderer->addLightRay(*this);

    for (auto& successor : *m_successors ) {
        successor->_synchronize(m_renderer);
    }
}

void LightRay::_synchronize(LightRayRenderer *renderer)
{
    delete m_renderer;
    m_renderer = nullptr;

    m_renderer->addLightRay(*this);

    for (auto& successor : *m_successors ) {
        successor->_synchronize(renderer);
    }
}

void LightRay::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;
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

Player * LightRay::player()
{
    return m_player;
}

void LightRay::setPlayer(Player *attachedPlayer)
{
    m_player = attachedPlayer;
}
