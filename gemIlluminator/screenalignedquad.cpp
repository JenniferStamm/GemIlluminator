#include <cassert>

#include "ScreenAlignedQuad.h"

#include <QOpenGLFunctions>


ScreenAlignedQuad::ScreenAlignedQuad() :
    m_vertices(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
{
    m_vertices->create();
    m_vertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    strip(*m_vertices);
}

ScreenAlignedQuad::~ScreenAlignedQuad()
{
    m_vertices->destroy();
    delete m_vertices;
}

void ScreenAlignedQuad::draw(QOpenGLFunctions &gl)
{
    m_vertices->bind();
    gl.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 0, nullptr);
    gl.glEnableVertexAttribArray(0);
	gl.glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void ScreenAlignedQuad::strip(QOpenGLBuffer &vertices)
{
	float rawv[] = { +1.f, -1.f , +1.f, +1.f , -1.f, -1.f , -1.f, +1.f };

    vertices.bind();
    vertices.allocate(rawv, sizeof(float)* 8);
}
