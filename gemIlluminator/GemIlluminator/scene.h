#ifndef SCENE_H
#define SCENE_H

#include <QQuickItem>
#include <QQmlListProperty>

class AbstractGeometry;
class SceneRenderer;

class Scene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(QQmlListProperty<AbstractGeometry> geometries READ geometries)

public:
    explicit Scene(QQuickItem *parent = 0);
    virtual ~Scene();

    QQmlListProperty<AbstractGeometry> geometries();
    qreal t();
    void setT(qreal t);

signals:
    void cubesChanged();
    void tChanged();

public slots:
    virtual void sync();
    virtual void cleanup();

protected:
    SceneRenderer *m_renderer;
    QList<AbstractGeometry*> m_geometries;
    qreal m_t;

private slots:
    void handleWindowChanged(QQuickWindow *win);
};

#endif // SCENE_H
