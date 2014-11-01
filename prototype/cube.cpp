#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>

#include "cube.h"

Cube::Cube()
    : m_t(0)
    , m_renderer(0)
{
    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));

}

void Cube::setT(qreal t)
{
    if(visible == m_visible) {
        return;
    }
    m_t = t;
    emit tChanged();
    if(window()) {
        window()->update();
    }
}

void Cube::setVisible(bool visible)
{
    if(visible == m_visible) {
        return;
    }
    m_visible = visible;
    emit visibleChanged();
    if(window()) {
        window()->update();
    }
}

void Cube::handleWindowChanged(QQuickWindow *win)
{
    if(win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void Cube::cleanup()
{
    if(m_renderer) {
        delete m_renderer;
        m_renderer = 0;
    }
}

void Cube::sync()
{
    if(!m_renderer) {
        m_renderer = new CubeRenderer();
        connect(window(), SIGNAL(beforeRendering()), m_renderer, SLOT(paint()), Qt::DirectConnection);
    }
    m_renderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    m_renderer->setT(m_t);
    m_renderer->setVisible(m_visible);
}

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

void CubeRenderer::paint()
{
    if(!m_program) {
        m_program = new QOpenGLShaderProgram(this);
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
        m_program->link();
    }
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    glViewport(0, 0, m_viewportSize.width(), m_viewportSize.height());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Needed to enable the depth test
    glDepthFunc(GL_LEQUAL);
    glDepthMask(true);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
    matrix.translate(0.25, 0.25, -2.5);
    matrix.rotate(m_frame, 1, 1, 1);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {
        0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
        //
        0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
        //
        0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
        //
        0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
        //
        0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
        //
        0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f,
    };

    GLfloat colors[] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        //
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        //
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,
        //
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        //
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f,
        //
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f
    };

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    if(m_visible) {
        glDrawArrays(GL_TRIANGLES, 0, 6);

        matrix.translate(0, 0, -1);
        m_program->setUniformValue(m_matrixUniform, matrix);
        glDrawArrays(GL_TRIANGLES, 6, 6);

        matrix.rotate(90, 0, 1, 0);
        matrix.translate(-0.5, 0, 0.5);
        m_program->setUniformValue(m_matrixUniform, matrix);
        glDrawArrays(GL_TRIANGLES, 12, 6);

        matrix.translate(0, 0, -1);
        m_program->setUniformValue(m_matrixUniform, matrix);
        glDrawArrays(GL_TRIANGLES, 18, 6);

        matrix.rotate(90, 1, 0, 0);
        matrix.translate(0, 0.5, 0.5);
        m_program->setUniformValue(m_matrixUniform, matrix);
        glDrawArrays(GL_TRIANGLES, 24, 6);

        matrix.translate(0, 0, -1);
        m_program->setUniformValue(m_matrixUniform, matrix);
        glDrawArrays(GL_TRIANGLES, 30, 6);
    }

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;
}

CubeRenderer::~CubeRenderer()
{
    delete m_program;
}
