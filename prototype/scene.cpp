#include "scene.h"

#include <QQuickWindow>

Scene::Scene(QQuickItem *parent) :
    QQuickItem(parent)
  , m_renderer(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

Scene::~Scene()
{

}

void Scene::sync()
{
    if (!m_renderer) {
        m_renderer = new SceneRenderer();
        connect(window(), SIGNAL(beforeRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
    }
    m_renderer->setViewport(window()->size() * window()->devicePixelRatio());
    m_renderer->setCubes(m_cubes);
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

void Scene::cleanup()
{
    if(m_renderer)
    {
        delete m_renderer;
        m_renderer = 0;
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

QQmlListProperty<Cube> Scene::cubes()
{
    return QQmlListProperty<Cube>(this, m_cubes);
}

int Scene::cubesCount() const
{
    return m_cubes.count();
}

Cube* Scene::cubeAt(int index) const
{
    return m_cubes.at(index);
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
    if(window())
        window()->update();
}
