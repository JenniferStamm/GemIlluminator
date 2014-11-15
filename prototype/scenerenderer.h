#ifndef RENDERERIMPLEMENTATION_H
#define RENDERERIMPLEMENTATION_H

#include <QObject>
#include <QSize>

// Qt sounds
#include <QMediaResource>
#include <QMediaContent>
#include <QMediaPlayer>

#include "cube.h"

class QEvent;
class QOpenGLFunctions;

class SceneRenderer : public QObject
{
    Q_OBJECT
public:
    explicit SceneRenderer(QObject *parent = 0);
    void setViewport(QSize viewport);
    void setCubes(QList<Cube*> cubes);

public slots:
    void paint();
    //bool event(QEvent *event);

protected:
    QList<Cube*> m_cubes;
    QSize m_viewport;
    QOpenGLFunctions *m_glFunctions;
    unsigned int m_frame;
    QMediaPlayer *m_player;
};

#endif // RENDERERIMPLEMENTATION_H
