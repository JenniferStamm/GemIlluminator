#include "lightray.h"

#include "abstractgem.h"
#include "camera.h"
#include "lightraydata.h"
#include "player.h"
#include "scene.h"
#include "soundmanager.h"
#include "triangle.h"

LightRay::LightRay(QObject *parent) :
    QObject(parent)
  , m_collidingGem(nullptr)
  , m_data(new LightRayData())
  , m_successors(new QList<LightRay *>)
  , m_selectedSuccessor(nullptr)
  , m_isStatic(false)
  , m_player(nullptr)
  , m_scene(nullptr)
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
}

void LightRay::update(int timeDifference)
{
    if (m_player) {
        QVector3D collisionPoint;
        LightRay collisionTestRay;
        collisionTestRay.setStartPosition(m_player->position());
        collisionTestRay.setEndPosition(endPosition());
        setCollidingGem(m_scene->findGemIntersectedBy(collisionTestRay, &collisionPoint));
        setEndPosition(collisionPoint);
    }

    //update before collision detection avoids problem of stack overflow while debugging
    //the problem is we don't reduce time each collision for movement on following rays
    for (auto& successor : *m_successors) {
        successor->update(timeDifference);
    }

    if (m_player) {
        calculateSuccessors();
        m_player->moveOnRay(*this, timeDifference);

        if (isPlayerBeforeCollisionPoint()) {
            LightRay collisionTestRay;
            collisionTestRay.setStartPosition(m_player->position());
            collisionTestRay.setEndPosition(endPosition());
            m_scene->setCurrentGem(m_scene->findGemWithBoundingSphereIntersectedBy(collisionTestRay));
        } else {
            Soundmanager::instance()->setCollisionSound(SoundEffects::Collision2);
            Soundmanager::instance()->playCollisionSound();
            m_collidingGem->setColor(m_data->color());
            m_scene->setCurrentGem(m_scene->findGemWithBoundingSphereIntersectedBy(*selectedSuccessor()));
            m_player->setPosition(endPosition());
            selectedSuccessor()->setPlayer(m_player);
            selectedSuccessor()->setColor(calculateSuccessorColor());
            m_player = nullptr;
            m_data->setColor(QVector3D(0.1f, 0.1f, 0.5f));
            setStatic();
        }

        QVector3D up = selectedSuccessor()->normalizedOrthogonalVector();
        m_scene->previewCamera()->setUp(up);
        m_scene->previewCamera()->setPosition(endPosition() + up * 0.1f);
        m_scene->previewCamera()->setViewDirection(selectedSuccessor()->direction());
    }
}

QVector3D LightRay::normalizedOrthogonalVector() const
{
    return m_data->normalizedOrthogonalVector();
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

const QVector3D &LightRay::direction() const
{
    return m_data->direction();
}

const QVector3D &LightRay::normalizedDirection() const
{
    return m_data->normalizedDirection();
}

AbstractGem *LightRay::collidingGem() const
{
    return m_collidingGem;
}

void LightRay::setCollidingGem(AbstractGem *gem)
{
    m_collidingGem = gem;
}

const QVector3D & LightRay::color() const
{
    return m_data->color();
}

void LightRay::setColor(const QVector3D &color)
{
    if (color == m_data->color()) {
        return;
    }
    m_data->setColor(color);
    emit colorChanged();
}

Player * LightRay::player() const
{
    return m_player;
}

void LightRay::setPlayer(Player *attachedPlayer)
{
    m_player = attachedPlayer;
    m_player->setPosition(startPosition());
    m_player->setViewDirection(direction());
}

Scene *LightRay::scene() const
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
    if (!m_selectedSuccessor) {
        if (m_successors->isEmpty()) {
            calculateSuccessors();
        }
        m_selectedSuccessor = m_successors->at(0);
    }
    return m_selectedSuccessor;
}

void LightRay::setSelectedSuccessor(LightRay *successor)
{
    m_selectedSuccessor = successor;
}

const QList<LightRay *> &LightRay::successors()
{
    return *m_successors;
}

void LightRay::calculateSuccessors()
{
    for (auto& successor : *m_successors) {
        delete successor;
    }
    m_successors->clear();
    m_selectedSuccessor = nullptr;

    auto collidingGem = m_scene->findGemIntersectedBy(*this);
    m_successors->append(collidingGem->processRayIntersection(*this, m_scene));
}

QVector3D LightRay::calculateSuccessorColor()
{
    QVector3D newColor = selectedSuccessor()->normalizedDirection();
    newColor /= 2.8f;
    newColor += QVector3D(0.45f, 0.45f, 0.45f);
    return newColor;
}

bool LightRay::isPlayerBeforeCollisionPoint()
{
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
    return !(factors.x() > 1.f || factors.y() > 1.f || factors.z() > 1.f);
}
