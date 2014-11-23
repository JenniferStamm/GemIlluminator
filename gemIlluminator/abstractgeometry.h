#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>

class AbstractGeometryRenderer;

class AbstractGeometry : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGeometry(QObject *parent = 0);
    virtual ~AbstractGeometry();

    virtual void synchronize();
    void paint();

protected:
    AbstractGeometryRenderer *m_renderer;
};

#endif // GEOMETRY_H
