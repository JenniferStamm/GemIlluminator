#include "scene.h"

#include <QQuickWindow>

#include "abstractgeometry.h"
#include "scenerenderer.h"

Scene::Scene(QQuickItem *parent) :
    QQuickItem(parent)
  , m_renderer(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

Scene::~Scene()
{
    delete m_renderer;
}

void Scene::sync()
{
    if (!m_renderer) {
        m_renderer = new SceneRenderer();
        connect(window(), SIGNAL(beforeRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
    }
    m_renderer->setViewport(window()->size() * window()->devicePixelRatio());
    m_renderer->setGeometries(m_geometries);
    m_renderer->setActive(m_active);

    for (QList<AbstractGeometry*>::iterator i = m_geometries.begin(); i != m_geometries.end(); i++) {
        (*i)->synchronize();
    }
}

void Scene::cleanup()
{
    if (m_renderer) {
        delete m_renderer;
        m_renderer = 0;
    }
    for (QList<AbstractGeometry*>::iterator i = m_geometries.begin(); i != m_geometries.end(); i++) {
        (*i)->cleanup();
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
    //geometry->setParent(this); TODO: Figure out, why this does not work or if we need it
    m_geometries.append(geometry);
    geometriesChanged();
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
