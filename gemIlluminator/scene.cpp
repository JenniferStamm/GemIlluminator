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
#include "scenerenderer.h"
#include "triangle.h"

Scene::Scene(QQuickItem *parent) :
    QQuickItem(parent)
  , m_renderer(nullptr)
  , m_navigation(nullptr)
  , m_bounds(new SceneBounds())
  , m_currentGem(m_bounds)
{
}

Scene::~Scene()
{
    delete m_bounds;
    delete m_renderer;
}

void Scene::sync()
{
    if (!m_renderer) {
        m_renderer = new SceneRenderer();
    }
    m_renderer->setGeometries(m_gem);

    for (auto& i : m_gem) {
        i->synchronize();
    }

}

void Scene::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = 0;
    }
    for (auto& i : m_gem) {
        i->cleanup();
    }
}

QQmlListProperty<AbstractGem> Scene::geometries()
{
    return QQmlListProperty<AbstractGem>(this, m_gem);
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

SceneRenderer& Scene::sceneRenderer() const
{
    assert(m_renderer);
    return *m_renderer;
}

AbstractGem *Scene::findGemIntersectedBy(const LightRay &ray, QVector3D *collisionPoint) const
{
    AbstractGem *result = m_bounds;
    float distance = m_bounds->intersectedBy(ray, collisionPoint);
    for( auto& gem : m_gem ){
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
    for( auto& gem : m_gem ){
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

Triangle *Scene::findGemFaceIntersectedBy(const LightRay &ray, QVector3D *collisionPoint) const
{
    Triangle *result;
    float distance = m_bounds->faceIntersectedBy(ray, result, collisionPoint);
    for (auto& gem : m_gem){
        QVector3D tempCollisionPoint;
        Triangle *tempResultTriangle;
        float collisionDistance = gem->faceIntersectedBy(ray, tempResultTriangle, &tempCollisionPoint);
        if (collisionDistance < distance) {
            distance = collisionDistance;
            if (collisionPoint) {
                *collisionPoint = tempCollisionPoint;
            }
            result = tempResultTriangle;
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
