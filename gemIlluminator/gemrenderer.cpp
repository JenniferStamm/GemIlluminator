#include "gemrenderer.h"

#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

GemRenderer::GemRenderer(QObject *parent):
    AbstractGeometryRenderer(parent)
,   m_vertices(new QOpenGLBuffer())
,   m_program(nullptr)
{

}

void GemRenderer::initialize()
{
    /*
    float vertexData[12] = {
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0, -0.5, -0.5,
        0.f, 0.5, 0.f};
        */

    float vertexData[72] = {
        // first triangle
        -0.5, -0.5, 0.5,
        1.0, 0.0, 0.0,
        0, -0.5, -0.5,
        1.0, 0.0, 0.0,
        0.f, 0.5, 0.f,
        1.0, 0.0, 0.0,
        // second triangle
        0, -0.5, -0.5,
        0.0, 1.0, 1.0,
        0.f, 0.5, 0.f,
        0.0, 1.0, 1.0,
        0.5, -0.5, 0.5,
        0.0, 1.0, 1.0,
        // third triangle
        0.f, 0.5, 0.f,
        0.0, 1.0, 0.0,
        0.5, -0.5, 0.5,
        0.0, 1.0, 0.0,
        -0.5, -0.5, 0.5,
        0.0, 1.0, 0.0,
        // fourth triangle
        0.5, -0.5, 0.5,
        0.0, 0.0, 1.0,
        -0.5, -0.5, 0.5,
        0.0, 0.0, 1.0,
        0, -0.5, -0.5,
        0.0, 0.0, 1.0};

    m_vertices->create();
    m_vertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertices->bind();
    m_vertices->allocate(vertexData, sizeof(float) * 72);

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vgem.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fgem.glsl");
    if (!m_program->link()) {
        //close();
    }
}

void GemRenderer::paint(QOpenGLFunctions *gl, QMatrix4x4 viewProjection)
{
    if (!m_program) {
        initialize();
    }

    m_vertices->bind();
    m_program->bind();

    QMatrix4x4 mvp = viewProjection;
    m_program->setUniformValue("modelViewProjection", mvp);

    gl->glEnableVertexAttribArray(0);
    gl->glEnableVertexAttribArray(1);

    m_program->bindAttributeLocation("a_vertex", 0);
    m_program->bindAttributeLocation("a_color", 1);

    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

    gl->glDrawArrays(GL_TRIANGLE_STRIP, 0, 12);

    gl->glDisableVertexAttribArray(0);
    gl->glDisableVertexAttribArray(1);

    m_vertices->release();
    m_program->release();
}
