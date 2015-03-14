#ifndef PAINTERQML_H
#define PAINTERQML_H

#include <QQuickItem>

class QTime;

class LightRay;
class Painter;
class Scene;

/**
 * @brief The PainterQML class is responsible to make our game visible within QML using Painter.
 * @detail This class is intended to be added and created within QML.
 * As the element showing our game it recognizes(?) resize events and update events needed by our application.
 * Also it keeps our rendering alive. Furthermore it is the interface between game logic and rendering.
 */
class PainterQML : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(bool isAppActive READ isAppActive WRITE setIsAppActive NOTIFY isAppActiveChanged)
    Q_PROPERTY(Scene* scene READ scene WRITE setScene NOTIFY sceneChanged)
public:
    explicit PainterQML(QQuickItem *parent = 0);
    virtual ~PainterQML();

    /**
     * @brief Process incomming qt events.
     * @param ev Event that should be handled.
     * @return Returns true if the event was handled by QObject.
     */
    bool event(QEvent *ev) override;

    /**
     * @brief Checks if our painter is active.
     * @detail Active means, that the game should be updated and rendered.
     * But updates and rendering will be done only if there are no reasons against (e.g. The app is not active)
     * @return Returns true if PainterQML is active.
     * @seealso isAppActive()
     */
    bool isActive() const;
    /**
     * @brief Sets active state of PainterQML.
     * @param active
     */
    void setActive(bool active);

    /**
     * @brief Queries registered event type, that should be send after rendering of current frame is done.
     * @return Returns registered event type
     */
    QEvent::Type paintingDoneEventType();

    /**
     * @brief Reloads enviroment map of game using Config
     */
    Q_INVOKABLE void reloadEnvMap();
    /**
     * @brief Resets update timer. This method is provided to allow manual resetting update time.
     */
    Q_INVOKABLE void resetTimer();

    /**
     * @brief The scene, that is painted by PainterQML
     * @return
     */
    Scene *scene() const;
    /**
     * @brief Set the scene, which should be drawn by PainterQML
     * @param scene
     */
    void setScene(Scene *scene);

    /**
     * @brief Check if the PainterQML assumes the app is active or not.
     * @return
     */
    bool isAppActive() const;
    /**
     * @brief Inform the painter if app is active or not. Setting this attribute to false leads to pause of game (stop updating and rendering).
     * @param active
     */
    void setIsAppActive(bool active);

signals:
    void activeChanged();
    void sceneChanged();
    void isAppActiveChanged();

protected slots:
    /**
     * @brief Synchronize game logic and rendering.
     * @detail Because QML uses different threads for ui and rendering, synchronization between these threads has to be done.
     * This slot is connected by PainterQML to corresponding signals of QQuickItem.
     */
    void synchronize();
    /**
     * @brief Clean up the rendering thread ressources.
     * @detail This slot is also connected by PainterQML to corresponding signal of QQuickItem.
     */
    void cleanup();

protected:
    bool m_active;
    bool m_isAppActive;
    bool m_isSceneDeletionRequired;
    bool m_isUpdatePending;
    Painter *m_painter;
    int m_paintingDoneEventType;
    Scene *m_scene;
    QTime *m_time;

private slots:
    void handleWindowChanged(QQuickWindow *win);
};

#endif // PAINTERQML_H
