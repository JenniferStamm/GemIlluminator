#include "soundmanager.h"

#include <QMutex>
#include <QMediaPlayer>
#include <QMediaPlaylist>

Soundmanager *Soundmanager::m_instance = 0;

Soundmanager::~Soundmanager()
{
    delete m_backgroundMusic->playlist();
    delete m_collisionSound->playlist();
    delete m_backgroundMusic;
    delete m_collisionSound;
}

void Soundmanager::drop()
{
    static QMutex mutex;
    mutex.lock();
    delete m_instance;
    m_instance = 0;
    mutex.unlock();
}

Soundmanager *Soundmanager::instance()
{
    static QMutex mutex;
    if (!m_instance) {
        mutex.lock();

        if (!m_instance) {
            m_instance = new Soundmanager;
            m_instance->loadSounds();
        }

        mutex.unlock();
    }

    return m_instance;
}

void Soundmanager::playBackgroundMusic()
{
    m_backgroundMusic->play();
}

void Soundmanager::setCollisionSound(SoundEffects effect)
{
    m_collisionSound->playlist()->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    m_collisionSound->playlist()->setCurrentIndex(effect);
}

void Soundmanager::playCollisionSound()
{
    m_collisionSound->play();
}

void Soundmanager::stopBackgroundMusic()
{
    m_backgroundMusic->stop();
}

void Soundmanager::loadSounds()
{
    // Load the background music playlist
    m_backgroundMusic = new QMediaPlayer;
    QMediaPlaylist *backgroundMusic = new QMediaPlaylist;
    backgroundMusic->addMedia(QUrl("qrc:/data/sound/background1.mp3"));
    backgroundMusic->setPlaybackMode(QMediaPlaylist::Loop);
    m_backgroundMusic->setPlaylist(backgroundMusic);
    m_backgroundMusic->setVolume(60);

    // Load the sounds played if a collision takes place
    m_collisionSound = new QMediaPlayer;
    QMediaPlaylist *collisionSound = new QMediaPlaylist;
    collisionSound->addMedia(QUrl("qrc:/data/sound/collision1.mp3"));
    collisionSound->addMedia(QUrl("qrc:/data/sound/collision2.mp3"));
    collisionSound->addMedia(QUrl("qrc:/data/sound/collision3.mp3"));
    collisionSound->addMedia(QUrl("qrc:/data/sound/collision4.mp3"));
    collisionSound->addMedia(QUrl("qrc:/data/sound/collision5.mp3"));
    collisionSound->addMedia(QUrl("qrc:/data/sound/collision6.mp3"));
    collisionSound->addMedia(QUrl("qrc:/data/sound/collision7.mp3"));
    collisionSound->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    m_collisionSound->setPlaylist(collisionSound);
    m_collisionSound->setVolume(100);

}

