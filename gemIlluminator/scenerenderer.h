#ifndef SCENERENDERER_H
#define SCENERENDERER_H

#include <QObject>
#include <QSize>

class SceneRenderer : public QObject
{
    Q_OBJECT
public:
    explicit SceneRenderer(QObject *parent = 0);
    void setViewport(QSize viewport);

    bool isActive();
    void setActive(bool active);

protected:
    QSize m_viewport;
    bool m_active;
};

#endif // SCENERENDERER_H
