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
    Q_PROPERTY(bool isAppActive READ isAppActive WRITE setIsAppActive)
    Q_PROPERTY(Scene* scene READ scene WRITE setScene NOTIFY sceneChanged)

public:
    explicit PainterQML(QQuickItem *parent = 0);
    virtual ~PainterQML();

    bool event(QEvent *ev) override;

    bool isActive() const;
    void setActive(bool active);

    QEvent::Type paintingDoneEventType();

    Q_INVOKABLE void reloadEnvMap();
    Q_INVOKABLE void resetTimer();

    Scene *scene() const;
    void setScene(Scene *scene);

    bool isAppActive() const;
    void setIsAppActive(bool active);

signals:
    void activeChanged();
    void sceneChanged();

public slots:
    void synchronize();
    void cleanup();

protected:
    bool m_active;
    bool m_isAppActive;
    bool m_isUpdatePending;
    Painter *m_painter;
    int m_paintingDoneEventType;
    Scene *m_scene;
    QTime *m_time;

private slots:
    void handleWindowChanged(QQuickWindow *win);
};

#endif // PAINTERQML_H
