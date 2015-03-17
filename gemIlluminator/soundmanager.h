#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QQmlEngine>

class QJSEngine;
class QMediaPlayer;
class QQmlEngine;

enum class SoundEffects {
    Collision1 = 0,
    Collision2 = 1,
    Collision3 = 2,
    Collision4 = 3,
    Collision5 = 4,
    Collision6 = 5,
    Collision7 = 6,
    GameOver = 7
};

/**
 * @brief The Soundmanager class provides several sounds which can be played.
 * @detail The Soundmanager manages the required resources.
 */
class Soundmanager : public QObject
{
    Q_OBJECT

    Soundmanager(const Soundmanager &) = delete;
    Soundmanager& operator=(const Soundmanager &) = delete;
public:
    virtual ~Soundmanager();

    /**
     * @brief Drops current instance of Soundmanager
     */
    static void drop();
    /**
     * @brief The instance of our Soundmanager
     * @return
     */
    static Soundmanager *instance();
    /**
      * @brief Starts playing our background music
      * @see stopBackgroundMusic()
      */
    Q_INVOKABLE void playBackgroundMusic();
    /**
     * @brief Plays a previously defined sound in order to indicate a collision
     * @see setCollisionSound()
     */
    void playCollisionSound();
    /**
     * @brief Sets the collision sound that will be played next time playCollisionSound() is called
     * @param effect The chosen SoundEffect
     */
    void setCollisionSound(SoundEffects effect);
    /**
      * @brief Stops playing our background music
      */
    Q_INVOKABLE void stopBackgroundMusic();

protected:
    Soundmanager();
    void loadSounds();

protected:
    static Soundmanager *m_instance;
    QMediaPlayer *m_backgroundMusic;
    QMediaPlayer *m_collisionSound;
};

/**
 * @brief Callback function used to get the current instance of Soundmanager within QML.
 * @param engine Unused parameter required by callback.
 * @param scriptEngine Unused parameter required by callback.
 * @return Our instance of Soundmanager
 */
QObject *soundmanagerSingletontypeProvider(QQmlEngine * engine, QJSEngine * scriptEngine);

#endif // SOUNDMANAGER_H
