#ifndef GAMELOSTRAY_H
#define GAMELOSTRAY_H

#include "lightray.h"

class GameLostRay : public LightRay
{
    Q_OBJECT
public:
    explicit GameLostRay(QObject *parent = 0);
    void update(int timeDifference) override;

signals:
    void gameLost();
};

#endif // GAMELOSTRAY_H
