#include "scene.h"

#include <QQuickWindow>
#include <QTime>

#include "abstractgeometry.h"
#include "camera.h"
#include "navigation.h"
#include "scenerenderer.h"

Scene::Scene(QQuickItem *parent) :
    QQuickItem(parent)
,   m_renderer(0)
,   m_time(new QTime())
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
    m_time->start();
}

Scene::~Scene()
{
    delete m_renderer;
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
        m_renderer->setActive(m_active);
        m_renderer->setViewProjection(m_camera->viewProjection());

        // As soon as we need time, we'll use this
        // int elapsedTime = m_time->restart();

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

QQmlListProperty<AbstractGeometry> Scene::geometries()
{
    return QQmlListProperty<AbstractGeometry>(this, m_geometries);
}

void Scene::appendGeometry(AbstractGeometry *geometry) {
    geometry->setParent(m_renderer);
    m_geometries.append(geometry);
    geometriesChanged();
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
