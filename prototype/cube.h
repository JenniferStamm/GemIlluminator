#ifndef CUBE_H
#define CUBE_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>

class CubeRenderer : public QObject {
    Q_OBJECT
public:
    CubeRenderer() : m_t(0), m_program(0) { }
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
