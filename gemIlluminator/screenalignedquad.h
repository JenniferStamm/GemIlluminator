#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class QOpenGLFunctions;

class ScreenAlignedQuad
{
public:
	ScreenAlignedQuad(
        QOpenGLFunctions &gl
	,	const GLuint vertexAttribLocation = 0);

    void draw(QOpenGLFunctions &gl);

protected:
    static void strip(QOpenGLBuffer &vertices);

private:
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vertices;
};
