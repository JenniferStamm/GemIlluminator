#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QQmlEngine>

class QMediaPlayer;

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

class Soundmanager : public QObject
{
    Q_OBJECT
public:
    virtual ~Soundmanager();

    static void drop();
    static Soundmanager *instance();
    Q_INVOKABLE void playBackgroundMusic();
    void playCollisionSound();
    void setCollisionSound(SoundEffects effect);
    Q_INVOKABLE void stopBackgroundMusic();

protected:
    Soundmanager() {}
    Soundmanager(const Soundmanager &); // hide copy constructor
    Soundmanager& operator=(const Soundmanager &); // hide assign op
                                 // we leave just the declarations, so the compiler will warn us
                                 // if we try to use those two functions by accident
    void loadSounds();

protected:
    static Soundmanager *m_instance;
    QMediaPlayer *m_backgroundMusic;
    QMediaPlayer *m_collisionSound;
};

static QObject *soundmanagerSingletontypeProvider(QQmlEngine * /*engine*/, QJSEngine * /*scriptEngine*/)
{
    Soundmanager *instance = Soundmanager::instance();
    return instance;
}

#endif // SOUNDMANAGER_H
