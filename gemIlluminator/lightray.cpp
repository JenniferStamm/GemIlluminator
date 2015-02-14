#include "lightray.h"

#include <cstdlib>

#include "abstractgem.h"
#include "lightraydata.h"
#include "lightrayrenderer.h"
#include "player.h"
#include "scene.h"
#include "triangle.h"

LightRay::LightRay(QObject *parent) :
    QObject(parent)
  , m_collidingGem(nullptr)
  , m_data(new LightRayData())
  , m_successors(new QList<LightRay *>)
  , m_selectedSuccessor(nullptr)
  , m_renderer(nullptr)
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

void LightRay::synchronize()
{
    m_renderer->addLightRay(*this);

    for (auto& successor : *m_successors ) {
        successor->synchronize();
    }
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
            m_scene->findGemWithBoundingSphereIntersectedBy(collisionTestRay);
            m_scene->setCurrentGem(m_collidingGem);
        } else {
            m_collidingGem->setColor(m_data->color());

            m_scene->setCurrentGem(m_scene->findGemWithBoundingSphereIntersectedBy(*selectedSuccessor()));
            m_player->setPosition(endPosition());
            selectedSuccessor()->setPlayer(m_player);
            m_player = nullptr;
            m_data->setColor(QVector3D(0.f, 0.f, 1.f));
            setStatic();
        }
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

QVector3D LightRay::direction() const
{
    return m_data->direction();
}

QVector3D LightRay::normalizedDirection() const
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

void LightRay::setRenderer(LightRayRenderer *renderer)
{
    m_renderer = renderer;

    for (auto& successor : *m_successors ) {
        successor->setRenderer(renderer);
    }
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
        calculateSuccessors();
        m_selectedSuccessor = m_successors->at(0);
    }
    return m_selectedSuccessor;
}

void LightRay::setSelectedSuccessor(LightRay *successor)
{
    m_selectedSuccessor = successor;
}

void LightRay::paint(QOpenGLFunctions &gl, const QMatrix4x4 &viewProjection, QOpenGLShaderProgram &shaderProgram)
{
    if (m_renderer) {
        m_renderer->paint(gl, viewProjection, shaderProgram);
    }
}

void LightRay::calculateSuccessors()
{
    for (auto& successor : *m_successors) {
        delete successor;
    }
    m_successors->clear();

    LightRay *nextRay = new LightRay();
    nextRay->setScene(m_scene);
    nextRay->setStartPosition(endPosition());

    Triangle *intersectedFace = m_scene->findGemFaceIntersectedBy(*this);
    QVector3D reflectedDirection = intersectedFace->reflect(direction());
    nextRay->setEndPosition(endPosition() + reflectedDirection * 10);

    QVector3D nextCollisionPoint;
    m_scene->findGemIntersectedBy(*nextRay, &nextCollisionPoint);
    nextRay->setEndPosition(nextCollisionPoint);
    m_successors->push_back(nextRay);
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
