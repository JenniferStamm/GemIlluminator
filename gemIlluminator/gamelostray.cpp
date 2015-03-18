#include "gamelostray.h"

#include "lightraydata.h"
#include "soundmanager.h"

GameLostRay::GameLostRay(QObject *parent) :
    LightRay(parent)
  , m_alreadyLost(false)
{
}

void GameLostRay::update(int timeDifference)
{
    if (m_player) {
        if (!m_alreadyLost) {
            emit gameLost();
            Soundmanager::instance()->setCollisionSound(SoundEffects::GameOver);
            Soundmanager::instance()->playCollisionSound();
            Soundmanager::instance()->stopBackgroundMusic();
            m_alreadyLost = true;
        }
    } else {
        LightRay::update(timeDifference);
    }
}
