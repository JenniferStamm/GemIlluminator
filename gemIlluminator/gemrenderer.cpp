#include "gemrenderer.h"

#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

    static const char *vertexShaderSource =
            "uniform mat4 modelViewProjection; \n"
            "attribute highp vec3 a_vertex; \n"
            "void main() \n"
            "{ \n"
            " gl_Position = modelViewProjection * vec4(a_vertex, 1.0); \n"
            "} \n";
    static const char *fragmentShaderSource =
            "void main() \n"
            "{ \n"
            " gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); \n"
            "} \n";

GemRenderer::GemRenderer(QObject *parent):
    AbstractGeometryRenderer(parent)
,   m_vertices(new QOpenGLBuffer())
,   m_indices(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer))
,   m_program(nullptr)
{
    float vertexData[12] = {
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0, -0.5, -0.5,
        0.f, 0.5, 0.f};
    uint indexData[6] = {
        0, 2, 3, 1, 0, 2
    };

    m_vertices->create();
    m_vertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vertices->bind();
    m_vertices->allocate(vertexData, sizeof(float) * 12);

    m_indices->create();
    m_indices->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indices->bind();
    m_indices->allocate(indexData, sizeof(uint) * 6);
}

void GemRenderer::paint(QOpenGLFunctions *gl)
{
    if (!m_program)
    {
        m_program = new QOpenGLShaderProgram(this);
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
        m_program->link();
    }
    m_vertices->bind();
    m_indices->bind();
    m_program->bind();

    QMatrix4x4 mvp;
    mvp.translate(0.f, 0.f, 0.5f);
    m_program->setUniformValue("modelViewProjection", mvp);

    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    gl->glEnableVertexAttribArray(0);

    gl->glDrawElements(GL_TRIANGLE_STRIP, m_indices->size() / sizeof(uint), GL_UNSIGNED_INT, nullptr);

    gl->glDisableVertexAttribArray(0);

    m_vertices->release();
    m_indices->release();
    m_program->release();
}
