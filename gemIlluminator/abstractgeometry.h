#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QObject>

class AbstractGeometryRenderer;
class QOpenGLFunctions;

class AbstractGeometry : public QObject
{
    Q_OBJECT
public:
    explicit AbstractGeometry(QObject *parent = 0);
    virtual ~AbstractGeometry();

    virtual void synchronize();
    void paint(QOpenGLFunctions *gl);

protected:
    AbstractGeometryRenderer *m_renderer;
};

#endif // GEOMETRY_H
