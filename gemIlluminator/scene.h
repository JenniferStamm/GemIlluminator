#ifndef SCENE_H
#define SCENE_H

#include <QQuickItem>
#include <QQmlListProperty>

class AbstractGeometry;
class SceneRenderer;
class AbstractNavigation;

class Scene : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(QQmlListProperty<AbstractGeometry> geometries READ geometries NOTIFY geometriesChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)

public:
    explicit Scene(QQuickItem *parent = 0);
    virtual ~Scene();

    QQmlListProperty<AbstractGeometry> geometries();

    qreal t();
    void setT(qreal t);

    bool isActive();

    void setActive(bool active);

signals:
    void cubesChanged();
    void tChanged();
    void geometriesChanged();
    void activeChanged();

public slots:
    virtual void sync();
    virtual void cleanup();
    void appendGeometry(AbstractGeometry *geometry);
    void registerNavigation(AbstractNavigation *navigation);

protected:
    SceneRenderer *m_renderer;
    QList<AbstractGeometry*> m_geometries;
    qreal m_t;
    bool m_active;
    AbstractNavigation *m_navigation;

private slots:
    void handleWindowChanged(QQuickWindow *win);
};

#endif // SCENE_H
