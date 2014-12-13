#ifndef GEM_H
#define GEM_H

#include "abstractgeometry.h"

template <typename T> class QVector;
class QVector3D;

class Gem : public AbstractGeometry
{
    Q_OBJECT
    Q_PROPERTY(QVector3D initialRotation READ initialRotation WRITE setInitialRotation NOTIFY initialRotationChanged)

public:
    explicit Gem(QObject *parent = 0);
    virtual ~Gem();

    virtual void synchronize() override;
    virtual void cleanup() override;
    virtual void update(int timeDifference) override;

    QVector3D initialRotation() const;
    void setInitialRotation(const QVector3D &initialRotation);

signals:
    void initialRotationChanged();

protected:
    QVector3D m_initialRotation;
    QVector<QVector3D> *m_vertices;
    QVector<QVector3D> *m_colors;
};

#endif // GEM_H
