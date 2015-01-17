#include "scene.h"

#include <limits>

#include <QQuickWindow>
#include <QTime>
#include <QVector3D>

#include "abstractgem.h"
#include "camera.h"
#include "lightray.h"
#include "navigation.h"
#include "scenerenderer.h"
#include "triangle.h"

Scene::Scene(QQuickItem *parent) :
    QQuickItem(parent)
  , m_renderer(nullptr)
  , m_time(nullptr)
  , m_currentGem(nullptr)
  , m_rootLightRay(new LightRay(this))
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
    m_rootLightRay->setStartPosition(QVector3D(0, 0, 0));
    m_rootLightRay->setEndPosition(QVector3D(0, 1, 0));
}

Scene::~Scene()
{
    delete m_renderer;
    delete m_time;
}

void Scene::sync()
{
    if (m_active) {
        if (!m_time) {
            m_time = new QTime();
            m_time->start();
        }

        if (!m_renderer) {
            m_renderer = new SceneRenderer();
            connect(window(), SIGNAL(beforeRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
        }

        m_renderer->setViewport(window()->size() * window()->devicePixelRatio());
        m_renderer->setGeometries(m_gem);
        m_renderer->setRootLightRay(m_rootLightRay);
        m_renderer->setActive(m_active);
        m_renderer->setViewProjection(m_camera->viewProjection());

        int elapsedTime = m_time->restart();

        m_rootLightRay->update(elapsedTime);
        m_rootLightRay->synchronize();

        for (auto& i : m_gem) {
            i->synchronize();
        }

        if (m_currentGem) {
            m_currentGem->setRotation(QVector3D(m_navigation->rotateX(),
                                                        m_navigation->rotateY(),
                                                        m_navigation->rotateZ()));
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
    m_navigation = navigation;
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
    AbstractGem *result = nullptr;
    QVector3D noCollisionPoint(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    if (collisionPoint) {
        *collisionPoint = noCollisionPoint;
    }
    float distance = std::numeric_limits<float>::max();
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
    return result;
}

AbstractGem *Scene::findGemFaceIntersectedBy(const LightRay &ray, int &faceIndex, QVector3D *collisionPoint) const
{
    AbstractGem *result = nullptr;
    faceIndex = -1;
    const float maxFloat = std::numeric_limits<float>::max();
    QVector3D noCollisionPoint(maxFloat, maxFloat, maxFloat);
    if (collisionPoint) {
        *collisionPoint = noCollisionPoint;
    }
    float distance = maxFloat;
    for (auto& gem : m_gem){
        QVector3D tempCollisionPoint;
        float collisionDistance = gem->faceIntersectedBy(ray, faceIndex, &tempCollisionPoint);
        if (collisionDistance < distance) {
            distance = collisionDistance;
            if (collisionPoint) {
                *collisionPoint = tempCollisionPoint;
            }
            result = gem;
        }
    }
    return result;
}

void Scene::setCurrentGem(AbstractGem *currentGem)
{
    m_currentGem = currentGem;
}
