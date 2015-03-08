#ifndef GAMELOSTRAY_H
#define GAMELOSTRAY_H

#include "lightray.h"

class GameLostRay : public LightRay
{
    Q_OBJECT
public:
    explicit GameLostRay(QObject *parent = 0);
    /**
     * @brief This method is like LightRay::update() with the addition, that the game end is detect a soon as the ray has a player->
     * @param timeDifference Time differnce to previous update.
     */
    void update(int timeDifference) override;

signals:
    /**
     * @brief The gameLost() signal is emitted at that moment the ray detects the player leaving the scene.
     */
    void gameLost();

protected:
    bool m_alreadyLost;
};

#endif // GAMELOSTRAY_H
