#include "painterqml.h"

#include <cassert>

#include <QEvent>
#include <QQuickWindow>
#include <QTime>

#include "lightray.h"
#include "painter.h"
#include "scene.h"

PainterQML::PainterQML(QQuickItem *parent) :
    QQuickItem(parent)
  , m_active(false)
  , m_isAppActive(true)
  , m_isSceneDeletionRequired(false)
  , m_isUpdatePending(false)
  , m_painter(nullptr)
  , m_paintingDoneEventType(-1)
  , m_time(nullptr)
{
    connect(this, &PainterQML::windowChanged, this, &PainterQML::handleWindowChanged);
}

PainterQML::~PainterQML()
{
    delete m_painter;
    delete m_time;
}

bool PainterQML::event(QEvent *ev)
{
    if (ev->type() == paintingDoneEventType()) {
        m_isUpdatePending = false;
        return true;
    }
    switch (ev->type()) {
    case QEvent::UpdateRequest: {
        if (m_active && !m_isUpdatePending && window()) {
            window()->update();
            m_isUpdatePending = true;
        }
        if (m_active && m_isAppActive) {
            QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
        }
        return QQuickItem::event(ev);
    }
    default:
        return QQuickItem::event(ev);
    }
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
    if (m_active == active) {
        return;
    }
    m_active = active;
    if (m_active && m_isAppActive) {
        m_isUpdatePending = false;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    } else {
        delete m_time;
        m_time = nullptr;
    }
    emit activeChanged();
}

QEvent::Type PainterQML::paintingDoneEventType()
{
    if (m_paintingDoneEventType == -1) {
        m_paintingDoneEventType = QEvent::registerEventType();
    }

    return static_cast<QEvent::Type>(m_paintingDoneEventType);
}

void PainterQML::reloadEnvMap()
{
    if (m_painter) {
       m_painter->updateEnvMap();
    }
}

void PainterQML::resetTimer()
{
    if (m_time) {
        m_time->restart();
    }
}

Scene* PainterQML::scene() const
{
    return m_scene;
}

void PainterQML::setScene(Scene *scene)
{
    if (m_painter) {
        if (m_scene) {
            disconnect(m_painter, &Painter::initializeDone, m_scene, &Scene::handleGameStarted);
        }
        if (scene) {
            connect(m_painter, &Painter::initializeDone, scene, &Scene::handleGameStarted, Qt::QueuedConnection);
        }
    }
    m_scene = scene;
    m_isSceneDeletionRequired = true;
}

bool PainterQML::isAppActive() const
{
    return m_isAppActive;
}

void PainterQML::setIsAppActive(bool active)
{
    if (m_isAppActive == active) {
        return;
    }
    m_isAppActive = active;
    if (m_active && m_isAppActive) {
        m_isUpdatePending = false;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    } else {
        delete m_time;
        m_time = nullptr;
    }

    emit isAppActiveChanged();
}

void PainterQML::synchronize()
{
    if (!m_painter) {
        m_painter = new Painter(this, nullptr);
        connect(window(), &QQuickWindow::beforeRendering, m_painter, &Painter::paint, Qt::DirectConnection);
        if (m_scene) {
            connect(m_painter, &Painter::initializeDone, m_scene, &Scene::handleGameStarted, Qt::QueuedConnection);
        }
    }

    m_painter->setActive(m_active && m_isAppActive);

    if (m_isSceneDeletionRequired) {
        m_painter->clearScene();
        m_isSceneDeletionRequired = false;
    }
    if (m_active && m_isAppActive) {
        if (!m_time) {
            m_time = new QTime();
            m_time->start();
        }
        int elapsedTime = m_time->restart();
        m_scene->update(elapsedTime);
        m_painter->synchronizeScene(m_scene);
    }
}

void PainterQML::cleanup()
{
    delete m_painter;
    m_painter = nullptr;
}
