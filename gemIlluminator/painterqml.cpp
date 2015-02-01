#include "painterqml.h"

#include <QQuickWindow>
#include <QTime>

#include "lightray.h"
#include "painter.h"
#include "scene.h"

PainterQML::PainterQML(QQuickItem *parent) :
    QQuickItem(parent)
  , m_active(false)
  , m_painter(nullptr)
  , m_time(nullptr)
{
    connect(this, &PainterQML::windowChanged, this, &PainterQML::handleWindowChanged);
}

PainterQML::~PainterQML()
{
    delete m_painter;
    delete m_time;
}

void PainterQML::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &PainterQML::synchronize, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this, &PainterQML::cleanup, Qt::DirectConnection);
        win->setClearBeforeRendering(false);
    }
}

bool PainterQML::isActive() const
{
    return m_active;
}

void PainterQML::setActive(bool active)
{
    m_active = active;
    if (!m_active) {
        delete m_time;
        m_time = nullptr;
    }

    emit activeChanged();
}

Scene* PainterQML::scene() const
{
    return m_scene;
}

void PainterQML::setScene(Scene *scene)
{
    m_scene = scene;
}

qreal PainterQML::t() const
{
    return m_t;
}

void PainterQML::setT(qreal t)
{
    if(m_t == t)
        return;
    m_t = t;
    emit tChanged();
    if(window()){
        window()->update();
    }
}

void PainterQML::synchronize()
{
    if (m_painter) {
        m_painter->setActive(m_active);
    }

    if (m_active) {
        if (!m_painter) {
            m_painter = new Painter();
            connect(window(), &QQuickWindow::beforeRendering, m_painter, &Painter::paint, Qt::DirectConnection);
        }

        if (!m_time) {
            m_time = new QTime();
            m_time->start();
        }

        int elapsedTime = m_time->restart();

        m_scene->sync(elapsedTime);
    }
}

void PainterQML::cleanup()
{
    delete m_painter;
    m_painter = nullptr;

    m_scene->cleanup();
}
