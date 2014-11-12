#include "scenerenderer.h"

#include <QCoreApplication>
#include <QEvent>
#include <QOpenGLFunctions>

SceneRenderer::SceneRenderer(QObject *parent) :
    QObject(parent)
  , m_glFunctions(new QOpenGLFunctions())
{
    m_glFunctions->initializeOpenGLFunctions();

    // Media resources are a bit tricky to deploy (or at least their url, relative paths will not work)
    // To make sure the correct url is used console.log(<id>.source) in qml is a quite nice way
    m_frame = 0;
    QMediaResource mediaResource(QUrl("qrc:/data/Camera_Shutter.wav"));
    QMediaContent mediaContent(mediaResource);
    m_player = new QMediaPlayer(this);
    m_player->setMedia(mediaContent);
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
    glClearColor(1.f, 1.f, 1.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_viewport.width(), m_viewport.height());

#ifdef __ANDROID__
    if(m_frame % 10000 == 9999) {
#else
    if(m_frame % 200000 == 199999) {
#endif
        m_player->play();
    }


    for(QList<Cube*>::iterator i = m_cubes.begin(); i != m_cubes.end(); i++)
    {
        (*i)->paint(m_glFunctions);
    }

    m_frame++;

    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

bool SceneRenderer::event(QEvent *event)
{
    switch(event->type())
    {
        case QEvent::UpdateRequest: paint(); return true;
        default: return QObject::event(event);
    }
}
