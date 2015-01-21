#include <cassert>

#include "ScreenAlignedQuad.h"

#include <QOpenGLFunctions>


ScreenAlignedQuad::ScreenAlignedQuad(
    QOpenGLFunctions &gl
,	const GLuint /*vertexAttribLocation*/)
: m_vertices(QOpenGLBuffer::VertexBuffer)
{
	m_vao.create();
	m_vao.bind();

	m_vertices.create();
	m_vertices.setUsagePattern(QOpenGLBuffer::StaticDraw);

	strip(m_vertices);

	gl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 0, nullptr);
	gl.glEnableVertexAttribArray(0);

	m_vao.release();
}

void ScreenAlignedQuad::draw(QOpenGLFunctions &gl)
{
	m_vao.bind();
	gl.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	m_vao.release();
}

void ScreenAlignedQuad::strip(QOpenGLBuffer &vertices)
{
	float rawv[] = { +1.f, -1.f , +1.f, +1.f , -1.f, -1.f , -1.f, +1.f };

	vertices.bind();
	vertices.allocate(rawv, sizeof(float)* 8);
}
