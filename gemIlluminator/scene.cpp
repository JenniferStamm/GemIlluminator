#include "scene.h"

#include <cassert>
#include <limits>

#include <QQuickWindow>
#include <QTime>
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
  , m_time(nullptr)
  , m_rootLightRay(new LightRay(this))
  , m_navigation(nullptr)
  , m_bounds(new SceneBounds())
  , m_currentGem(m_bounds)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
    m_rootLightRay->setStartPosition(QVector3D(0, 0, 0));
    m_rootLightRay->setEndPosition(QVector3D(0, 1, 0));
}

Scene::~Scene()
{
    delete m_bounds;
    delete m_renderer;
    delete m_time;
}

void Scene::sync()
{
    if (m_renderer) {
        m_renderer->setActive(m_active);
    }
    if (m_active) {
        if (!m_time) {
            m_time = new QTime();
            m_time->start();
        }

        if (!m_renderer) {
            m_renderer = new SceneRenderer();
            connect(window(), SIGNAL(beforeRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
            m_renderer->setActive(m_active);
        }
        m_renderer->setViewport(window()->size() * window()->devicePixelRatio());
        m_renderer->setGeometries(m_gem);
        m_renderer->setRootLightRay(m_rootLightRay);
        m_renderer->setCamera(*m_camera);

        int elapsedTime = m_time->restart();

        m_rootLightRay->update(elapsedTime);
        m_rootLightRay->synchronize();

        for (auto& i : m_gem) {
            i->synchronize();
        }
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

void Scene::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        win->setClearBeforeRendering(false);
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

qreal Scene::t() const
{
    return m_t;
}

void Scene::setT(qreal t)
{
    if(m_t == t)
        return;
    m_t = t;
    emit tChanged();
    if(window()){
        window()->update();
    }
}

bool Scene::isActive() const
{
    return m_active;
}

void Scene::setActive(bool active)
{
    m_active = active;
    if (!m_active) {
        delete m_time;
        m_time = nullptr;
    }

    emit activeChanged();
}

Camera* Scene::camera() const
{
    return m_camera;
}

void Scene::setCamera(Camera* camera)
{
    m_camera = camera;
}

LightRay* Scene::rootLightRay() const
{
    return m_rootLightRay;
}

void Scene::setRootLightRay(LightRay *root)
{
    m_rootLightRay = root;
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
