#include "gamelostray.h"

#include "lightraydata.h"

GameLostRay::GameLostRay(QObject *parent) :
    LightRay(parent)
{
}

void GameLostRay::update(int timeDifference)
{
    if (m_player) {
        emit gameLost();
    } else {
        LightRay::update(timeDifference);
    }
}
