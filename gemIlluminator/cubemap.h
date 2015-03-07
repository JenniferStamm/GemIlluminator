#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <QObject>

class QOpenGLFunctions;

class CubeMap : public QObject
{
    Q_OBJECT
public:
    CubeMap(QOpenGLFunctions &gl, QString cubeMapPrefix, QObject *parent = 0);
    virtual ~CubeMap();

    void update(QString newCubeMapPrefix);

    uint cubeMapTexture();

protected:
    void initialize();


protected:
    uint m_cubeMap;
    QOpenGLFunctions &m_gl;
    QString m_cubeMapPrefix;


};

#endif // CUBEMAP_H
