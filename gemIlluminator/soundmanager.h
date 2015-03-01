#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QQmlEngine>

class QMediaPlayer;

enum SoundEffects {
    Collision1,
    Collision2,
    Collision3,
    Collision4,
    Collision5,
    Collision6,
    Collision7
};

class Soundmanager : public QObject
{
    Q_OBJECT
public:
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

static QObject *soundmanagerSingletontypeProvider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    Soundmanager *instance = Soundmanager::instance();
    return instance;
}

#endif // SOUNDMANAGER_H
