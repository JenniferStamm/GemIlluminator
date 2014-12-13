#ifndef LIGHTRAY_H
#define LIGHTRAY_H

#include "abstractgeometry.h"

template <typename T> class QList;
class Player;

class LightRay : public AbstractGeometry
{
    Q_OBJECT
public:
    explicit LightRay(QObject *parent = 0);

    virtual void synchronize();
    virtual void cleanup();
    virtual void update(int timeDifference);

signals:

public slots:

protected:
    QList<LightRay *> *m_successors;
    Player *m_player;
};

#endif // LIGHTRAY_H
