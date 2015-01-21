#pragma once

#include <QOpenGLBuffer>

class QOpenGLFunctions;

class ScreenAlignedQuad
{
public:
    ScreenAlignedQuad();
    ~ScreenAlignedQuad();

    void draw(QOpenGLFunctions &gl);

protected:
    static void strip(QOpenGLBuffer &vertices);

private:
	QOpenGLBuffer m_vertices;
};
