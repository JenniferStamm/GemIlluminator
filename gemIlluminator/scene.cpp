#include "scene.h"

#include <QQuickWindow>
#include <QTime>
#include <QVector3D>

#include "abstractgem.h"
#include "camera.h"
#include "lightray.h"
#include "navigation.h"
#include "scenerenderer.h"

Scene::Scene(QQuickItem *parent) :
    QQuickItem(parent)
  , m_renderer(0)
  , m_time(new QTime())
  , m_rootLightRay(new LightRay(this))
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
    m_time->start();
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
        if (!m_renderer) {
            m_renderer = new SceneRenderer();
            connect(window(), SIGNAL(beforeRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
        }

        m_renderer->setViewport(window()->size() * window()->devicePixelRatio());
        m_renderer->setGeometries(m_geometries);
        m_renderer->setRootLightRay(m_rootLightRay);
        m_renderer->setActive(m_active);
        m_renderer->setViewProjection(m_camera->viewProjection());

        int elapsedTime = m_time->restart();

        m_rootLightRay->update(elapsedTime);
        m_rootLightRay->synchronize();

        for (auto& i : m_geometries) {
            i->synchronize();
            i->setRotation(QVector3D(m_navigation->rotateX(), m_navigation->rotateY(), m_navigation->rotateZ()));
        }
    }
}

void Scene::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = 0;
    }
    for (auto& i : m_geometries) {
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
    return QQmlListProperty<AbstractGem>(this, m_geometries);
}

void Scene::registerNavigation(Navigation *navigation)
{
    m_navigation = navigation;
}

qreal Scene::t()
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

bool Scene::isActive()
{
    return m_active;
}

void Scene::setActive(bool active)
{
    m_active = active;

    emit activeChanged();
}

Camera* Scene::camera()
{
    return m_camera;
}

void Scene::setCamera(Camera* camera)
{
    m_camera = camera;
}

LightRay* Scene::rootLightRay()
{
    return m_rootLightRay;
}

void Scene::setRootLightRay(LightRay *root)
{
    m_rootLightRay = root;
}
