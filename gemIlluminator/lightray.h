#ifndef LIGHTRAY_H
#define LIGHTRAY_H

#include "abstractgeometry.h"

template <typename T> class QList;
class Player;

class LightRay : public AbstractGeometry
{
    Q_OBJECT
    Q_PROPERTY(const QVector3D & startPosition READ startPosition WRITE setStartPosition NOTIFY startPositionChanged)
    Q_PROPERTY(const QVector3D & endPosition READ endPosition WRITE setEndPosition NOTIFY endPositionChanged)
    Q_PROPERTY(const QVector3D & direction READ direction)

public:
    explicit LightRay(QObject *parent = 0);
    virtual ~LightRay();

    virtual void synchronize();
    virtual void cleanup();
    virtual void update(int timeDifference);

    void setPlayer(Player *attachedPlayer);

signals:
    void startPositionChanged();
    void endPositionChanged();

public slots:
    const QVector3D & startPosition();
    void setStartPosition(const QVector3D & position);

    const QVector3D & endPosition();
    void setEndPosition(const QVector3D & position);

    const QVector3D & direction();

protected:
    QList<LightRay *> *m_successors;
    Player *m_player;
    QVector3D *m_startPosition;
    QVector3D *m_endPosition;
    QVector3D *m_direction;
};

#endif // LIGHTRAY_H
