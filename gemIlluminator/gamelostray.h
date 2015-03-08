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

protected:
    bool m_alreadyLost;
};

#endif // GAMELOSTRAY_H
