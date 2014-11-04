#ifndef CUBE_H
#define CUBE_H

// Qt window/quick stuff
#include <QWindow>
#include <QtQuick/qquickwindow.h>
#include <QtQuick/QQuickItem>
#include <QtGui/QScreen>

// OpenGL stuff
#include <QOpenGLFunctions>
#include <QtGui/QOpenGLShaderProgram>

// Qt sounds
#include <QMediaResource>
#include <QMediaContent>
#include <QMediaPlayer>

class CubeRenderer : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit CubeRenderer(QWindow *parent = 0);
    ~CubeRenderer();

    void setT(qreal t) { m_t = t; }
    void setVisible(bool visible) { m_visible = visible; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }

public slots:
    void paint();

private:
    QSize m_viewportSize;
    qreal m_t;
    bool m_visible;
    QOpenGLShaderProgram *m_program;
    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    int m_frame;
};

class Cube : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)

public:
    Cube();

    qreal t() const { return m_t; }
    void setT(qreal t);
    bool visible() const { return m_visible; }
    void setVisible(bool visible);

signals:
    void tChanged();
    void visibleChanged();

public slots:
    void sync();
    void cleanup();

private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    qreal m_t;
    bool m_visible;
    CubeRenderer *m_renderer;
};

#endif // CUBE_H
