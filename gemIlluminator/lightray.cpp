#include "lightray.h"

#include <cstdlib>

#include "lightraydata.h"
#include "lightrayrenderer.h"
#include "player.h"
#include "scene.h"

LightRay::LightRay(QObject *parent) :
    QObject(parent)
  , m_successors(new QList<LightRay *>)
  , m_selectedSuccessor(nullptr)
  , m_renderer(nullptr)
  , m_isStatic(false)
  , m_player(nullptr)
  , m_scene(nullptr)
  , m_data(new LightRayData())
{
}

LightRay::~LightRay()
{
    if (m_successors) {
        for (auto& successor : *m_successors ) {
            delete successor;
        }
    }
    delete m_successors;
    delete m_data;
    delete m_renderer;
}

void LightRay::synchronize()
{
    if (!m_renderer) {
        m_renderer = new LightRayRenderer();
    }

    if (m_scene) {
        m_renderer->setCamera(*m_scene->camera());
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

    renderer->addLightRay(*this);

    for (auto& successor : *m_successors ) {
        successor->_synchronize(renderer);
    }
}

void LightRay::calculateSuccessors()
{
    if (m_successors->isEmpty()) {
        LightRay *temp = new LightRay();
        temp->setScene(m_scene);
        //temp->setStartPosition(QVector3D(0.f, 0.f, 10.f));
        //temp->setEndPosition(QVector3D(0.f, 0.f, -10.f));
        temp->setStartPosition(endPosition());
        temp->setEndPosition(QVector3D(rand() % 21 - 10, rand() % 21 - 10, rand() % 21 - 10));
        QVector3D collisionPoint;
        if (m_scene->rayIntersectsTriangle(*temp, &collisionPoint)) {
            temp->setEndPosition(collisionPoint);
        }
        m_successors->push_back(temp);
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

    //update before collision detection avoids problem of stack overflow while debugging
    //the problem is we don't reduce time each collision for movement on following rays
    for (auto& successor : *m_successors) {
        successor->update(timeDifference);
    }

    if (m_player) {
        m_player->moveOnRay(*this, timeDifference);
        QVector3D playerPosition = m_player->position();
        QVector3D difference = playerPosition - startPosition();
        QVector3D factors;
        if (direction().x() != 0.f) {
            factors.setX(difference.x() / direction().x());
        }
        if (direction().y() != 0.f) {
            factors.setY(difference.y() / direction().y());
        }
        if (direction().z() != 0.f) {
            factors.setZ(difference.z() / direction().z());
        }
        if (factors.x() > 1.f || factors.y() > 1.f || factors.z() > 1.f)
        {
            playerPosition = endPosition();
            m_player->setPosition(playerPosition);
            selectedSuccessor()->setPlayer(m_player);
            m_player = nullptr;
            setStatic();
        } else {
            m_player->setPosition(playerPosition);
        }
    }
}

QVector3D LightRay::normalizedOrthogonalVector() const
{
    LightRayData temp(*this);
    return temp.normalizedOrthogonalVector();
}

const QVector3D & LightRay::startPosition() const
{
    return m_data->startPosition();
}

void LightRay::setStartPosition(const QVector3D &position)
{
    if (m_data->startPosition() == position) {
        return;
    }
    m_data->setStartPosition(position);
    emit startPositionChanged();
}

const QVector3D & LightRay::endPosition() const
{
    return m_data->endPosition();
}

void LightRay::setEndPosition(const QVector3D &position)
{
    if (m_data->endPosition() == position) {
        return;
    }
    m_data->setEndPosition(position);
    emit endPositionChanged();
}

const QVector3D & LightRay::direction() const
{
    return m_data->direction();
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

LightRay *LightRay::selectedSuccessor()
{
    if (m_selectedSuccessor) {
        return m_selectedSuccessor;
    } else {
        calculateSuccessors();
        return m_successors->at(0);
    }
}

void LightRay::setSelectedSuccessor(LightRay *successor)
{
    m_selectedSuccessor = successor;
}

void LightRay::paint(QOpenGLFunctions *gl)
{
    if (m_renderer) {
        m_renderer->paint(gl);
    }
}

const QVector3D & LightRay::normalizedDirection() const
{
    return m_data->normalizedDirection();
}
