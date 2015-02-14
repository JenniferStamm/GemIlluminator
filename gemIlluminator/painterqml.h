#ifndef PAINTERQML_H
#define PAINTERQML_H

#include <QQuickItem>

class QTime;

class LightRay;
class Painter;
class Scene;

class PainterQML : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QString envMapPrefix READ envMapPrefix WRITE setEnvMapPrefix NOTIFY envMapPrefixChanged)
    Q_PROPERTY(Scene* scene READ scene WRITE setScene NOTIFY sceneChanged)
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)

public:
    explicit PainterQML(QQuickItem *parent = 0);
    virtual ~PainterQML();

    QString envMapPrefix() const;
    void setEnvMapPrefix(const QString &envMapPrefix);

    bool isActive() const;
    void setActive(bool active);

    Scene *scene() const;
    void setScene(Scene *scene);

    qreal t() const;
    void setT(qreal t);

signals:
    void activeChanged();
    void sceneChanged();
    void tChanged();
    void envMapPrefixChanged();

public slots:
    void synchronize();
    void cleanup();

protected:
    bool m_active;
    QString m_envMapPrefix;
    bool m_isEnvMapInvalidated;
    Painter *m_painter;
    Scene *m_scene;
    qreal m_t;
    QTime *m_time;

private slots:
    void handleWindowChanged(QQuickWindow *win);
};

#endif // PAINTERQML_H
