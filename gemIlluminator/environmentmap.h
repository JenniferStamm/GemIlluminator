#ifndef ENVIRONMENTMAP_H
#define ENVIRONMENTMAP_H

#include <QObject>

class QOpenGLFunctions;
class QOpenGLShaderProgram;

class Camera;
class ScreenAlignedQuad;

class EnvironmentMap : public QObject
{
    Q_OBJECT
public:
    EnvironmentMap(QOpenGLFunctions &gl, QObject *parent = 0);
    virtual ~EnvironmentMap();

    void initialize();

    void paint(const Camera &camera);

    uint envMapTexture();

protected:
    void initializeShaderProgram();

protected:
    uint m_envMap;
    QOpenGLFunctions &m_gl;
    bool m_initialized;
    ScreenAlignedQuad *m_quad;
    QOpenGLShaderProgram *m_shaderProgram;
};

#endif // ENVIRONMENTMAP_H
