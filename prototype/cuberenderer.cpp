#include "cuberenderer.h"

#include <QOpenGLShaderProgram>

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

CubeRenderer::CubeRenderer(QObject *parent) :
    QObject(parent)
  , m_program(nullptr)
{

}

CubeRenderer::~CubeRenderer()
{
    delete m_program;
}

void CubeRenderer::setPosition(QVector3D position)
{
    m_position = position;
}

void CubeRenderer::setRotation(QVector3D rotation)
{
    m_rotation = rotation;
}

void CubeRenderer::paint(QOpenGLFunctions *gl)
{
    // Proper way to play sound is needed
    // ---------------------------------------------------------------------------
    // Media resources are a bit tricky to deploy (or at least their url, relative paths will not work)
    // To make sure the correct url is used console.log(<id>.source) in qml is a quite nice way
    // QMediaResource mediaResource(QUrl("qrc:/data/Camera_Shutter.wav"));
    // QMediaContent mediaContent(mediaResource);
    // QMediaPlayer *player = new QMediaPlayer(this);
    // player->setMedia(mediaContent);
    // player->play();

    if(!m_program) {
        m_program = new QOpenGLShaderProgram(this);
        m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
        m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
        m_program->link();
    }
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    gl->glEnable(GL_DEPTH_TEST);
    gl->glDepthFunc(GL_LEQUAL);
    gl->glDepthMask(GL_TRUE);

    m_program->bind();

    QMatrix4x4 matrix;

    //Both following lines should be done by a camera and not inside scene elements
    matrix.perspective(60.0f, 640.f / 480.f, 0.1f, 100.0f);
    matrix.translate(0.f, 0.f, -10.f);

    matrix.translate(m_position);
    matrix.rotate(m_rotation.x(), 1.0, 0.0, 0.0);
    matrix.rotate(m_rotation.y(), 0.0, 1.0, 0.0);
    matrix.rotate(m_rotation.z(), 0.0, 0.0, 1.0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        //
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        //
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        //
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        //
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        //
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
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

    gl->glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    gl->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    gl->glEnableVertexAttribArray(0);
    gl->glEnableVertexAttribArray(1);

    gl->glDrawArrays(GL_TRIANGLES, 0, 72);

    gl->glDisableVertexAttribArray(1);
    gl->glDisableVertexAttribArray(0);

    m_program->release();

    gl->glDisable(GL_DEPTH_TEST);
}
