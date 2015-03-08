#include "scene.h"

#include <cassert>
#include <limits>

#include <QQuickWindow>
#include <QVector3D>

#include "abstractgem.h"
#include "camera.h"
#include "lightray.h"
#include "navigation.h"
#include "scenebounds.h"
#include "triangle.h"

Scene::Scene(QQuickItem *parent) :
    QQuickItem(parent)
  , m_bounds(new SceneBounds())
  , m_camera(nullptr)
  , m_previewCamera(nullptr)
  , m_currentGem(m_bounds)
  , m_navigation(nullptr)
  , m_rootLightRay(nullptr)
{
}

Scene::~Scene()
{
    delete m_bounds;
}

QList<AbstractGem *> Scene::geometries()
{
    return m_gems;
}

void Scene::update(int elapsedTime)
{
    m_rootLightRay->update(elapsedTime);
}

void Scene::handleGameLost()
{
    emit gameLost();
}

void Scene::handleGameStarted()
{
    emit gameStarted();
}

QQmlListProperty<AbstractGem> Scene::geometriesQML()
{
    return QQmlListProperty<AbstractGem>(this, m_gems);
}

void Scene::registerNavigation(Navigation *navigation)
{
    if (m_navigation) {
        disconnect(m_navigation, &Navigation::worldSpaceRotationChanged, this, &Scene::rotateCurrentGem);
    }
    m_navigation = navigation;
    m_navigation->setCamera(m_camera);
    connect(m_navigation, &Navigation::worldSpaceRotationChanged, this, &Scene::rotateCurrentGem);
}

void Scene::rotateCurrentGem(const QQuaternion &quaternion)
{
    m_currentGem->rotate(quaternion);
}

Camera* Scene::camera() const
{
    return m_camera;
}

void Scene::setCamera(Camera* camera)
{
    m_camera = camera;
}

Camera* Scene::previewCamera() const
{
    return m_previewCamera;
}

void Scene::setPreviewCamera(Camera* camera)
{
    m_previewCamera = camera;
}

LightRay* Scene::rootLightRay() const
{
    return m_rootLightRay;
}

void Scene::setRootLightRay(LightRay *rootLightRay)
{
    m_rootLightRay = rootLightRay;
}

AbstractGem *Scene::findGemIntersectedBy(const LightRay &ray, QVector3D *collisionPoint) const
{
    AbstractGem *result = m_bounds;
    float distance = m_bounds->intersectedBy(ray, collisionPoint);
    for( auto& gem : m_gems ){
        QVector3D temp;
        float collisionDistance = gem->intersectedBy(ray, &temp);
        if (collisionDistance < distance) {
            distance = collisionDistance;
            if (collisionPoint) {
                *collisionPoint = temp;
            }
            result = gem;
        }
    }
    assert(result);
    return result;
}

AbstractGem *Scene::findGemWithBoundingSphereIntersectedBy(const LightRay &ray, QVector3D *collisionPoint) const
{
    AbstractGem *result = m_bounds;
    float distance = m_bounds->boundingSphereIntersectedBy(ray, collisionPoint);
    for( auto& gem : m_gems ){
        QVector3D temp;
        float collisionDistance = gem->boundingSphereIntersectedBy(ray, &temp);
        if (collisionDistance < distance) {
            distance = collisionDistance;
            if (collisionPoint) {
                *collisionPoint = temp;
            }
            result = gem;
        }
    }
    assert(result);
    return result;
}

void Scene::setCurrentGem(AbstractGem *currentGem)
{
    if (currentGem == nullptr) {
        return;
    }
    m_currentGem = currentGem;
}
