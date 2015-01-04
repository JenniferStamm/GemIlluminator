#include "lightray.h"

#include "lightrayrenderer.h"
#include "player.h"
#include "scene.h"

LightRay::LightRay(QObject *parent) :
    QObject(parent)
  , m_successors(new QList<LightRay*>())
  , m_renderer(nullptr)
  , m_isStatic(false)
  , m_player(nullptr)
  , m_scene(nullptr)
  , m_startPosition(new QVector3D())
  , m_endPosition(new QVector3D())
  , m_direction(new QVector3D())
  , m_normalizedDirection(new QVector3D())
{
}

LightRay::~LightRay()
{
    for (auto& successor : *m_successors ) {
        delete successor;
    }
    delete m_successors;
    delete m_startPosition;
    delete m_endPosition;
    delete m_direction;
    delete m_normalizedDirection;
    delete m_renderer;
}

void LightRay::synchronize()
{
    if (!m_renderer) {
        m_renderer = new LightRayRenderer();
    }

    m_renderer->setCamera(*m_player->camera());
    m_renderer->addLightRay(*this);

    for (auto& successor : *m_successors ) {
        successor->_synchronize(m_renderer);
    }
}

void LightRay::_synchronize(LightRayRenderer *renderer)
{
    delete m_renderer;
    m_renderer = nullptr;

    renderer->addLightRay(*this);

    for (auto& successor : *m_successors ) {
        successor->_synchronize(renderer);
    }
}

void LightRay::cleanup()
{
    delete m_renderer;
    m_renderer = nullptr;

    for (auto& successor : *m_successors) {
        successor->cleanup();
    }
}

void LightRay::update(int timeDifference)
{
    if (!isStatic() && m_scene) {
        QVector3D collisionPoint;
        if (m_scene->rayIntersection(*this, &collisionPoint)) {
            setEndPosition(collisionPoint);
        }
    }

    if (m_player) {
        QVector3D playerPosition = m_player->position();
        playerPosition += (normalizedDirection() * m_player->velocity() * timeDifference) / 1000;

        QVector3D difference = playerPosition - endPosition();
        if (difference.x() < 0.f || difference.y() < 0.f || difference.z() < 0.f)
        {
            playerPosition = endPosition();
        }

        m_player->setPosition(playerPosition);
    }

    for (auto& successor : *m_successors) {
        successor->update(timeDifference);
    }
}

const QVector3D & LightRay::startPosition() const
{
    return *m_startPosition;
}

void LightRay::setStartPosition(const QVector3D &position)
{
    if (*m_startPosition == position) {
        return;
    }
    *m_startPosition = position;
    setDirection(*m_endPosition - *m_startPosition);
    emit startPositionChanged();
}

const QVector3D & LightRay::endPosition() const
{
    return *m_endPosition;
}

void LightRay::setEndPosition(const QVector3D &position)
{
    if (*m_endPosition == position) {
        return;
    }
    *m_endPosition = position;
    setDirection(*m_endPosition - *m_startPosition);
    emit endPositionChanged();
}

const QVector3D & LightRay::direction() const
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
    m_player->setPosition(startPosition());
    m_player->setViewDirection(direction());
}

Scene *LightRay::scene()
{
    return m_scene;
}

void LightRay::setScene(Scene *owningScene)
{
    if (m_scene == owningScene) {
        return;
    }

    m_scene = owningScene;
    emit sceneChanged();
}

bool LightRay::isStatic() const
{
    return m_isStatic;
}

void LightRay::setStatic()
{
    m_isStatic = true;
}

void LightRay::paint(QOpenGLFunctions *gl)
{
    if (m_renderer) {
        m_renderer->paint(gl);
    }
}

const QVector3D & LightRay::normalizedDirection() const
{
    return *m_normalizedDirection;
}

void LightRay::setDirection(const QVector3D direction)
{
    *m_direction = direction;
    *m_normalizedDirection = direction.normalized();
}
