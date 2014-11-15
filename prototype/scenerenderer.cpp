#include "scenerenderer.h"

#include <QCoreApplication>
#include <QEvent>
#include <QOpenGLFunctions>
#include <QDebug>

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
  , m_glFunctions(new QOpenGLFunctions())
  , m_frame(0)
{
    m_glFunctions->initializeOpenGLFunctions();
}

void SceneRenderer::setViewport(QSize viewport)
{
    m_viewport = viewport;
}

void SceneRenderer::setCubes(QList<Cube*> cubes)
{
    m_cubes = cubes;
}

void SceneRenderer::paint()
{
    m_frame++;
    qDebug() << m_frame;
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_viewport.width(), m_viewport.height());

    for(QList<Cube*>::iterator i = m_cubes.begin(); i != m_cubes.end(); i++)
    {
        (*i)->paint(m_glFunctions);
    }
}

//bool SceneRenderer::event(QEvent *event)
//{
//    switch(event->type())
//    {
//        case QEvent::UpdateRequest: paint(); return true;
//        default: return QObject::event(event);
//    }
//}
