#include "GLWidget.hh"

#include "glm/gtc/matrix_transform.hpp"
#include <QOpenGLShader>
#include <iostream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), program(NULL)
{
    setFocusPolicy(Qt::StrongFocus);
}

GLWidget::~GLWidget()
{
    if (program != NULL)
        delete program;
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1.0);

    initValues();
    loadShaders();
    createBuffers();
}

void GLWidget::initValues()
{
    data.load("data/map.osm");
    boundaries = data.getBoundaries();
    sceneCenter = glm::vec3((boundaries[2] - boundaries[0]) / 2.0 + boundaries[0], (boundaries[3] - boundaries[1]) / 2.0 + boundaries[1], 0);

    vaos = vector<GLuint>(data.getNWays());

    zoomLevel = 10;
    movement = zoomLevel / 8;
}

void GLWidget::drawWays()
{
    modelTransform({0, 0, 0}, 0, 1);

    projectTransform();

    for (uint i = 0; i < vaos.size(); ++i)
    {
        glBindVertexArray(vaos[i]);

        glDrawArrays(GL_LINE_STRIP, 0, data.getWaySize(i));

        glBindVertexArray(0);
    }
}

void GLWidget::paintGL()
{
#ifdef __APPLE__
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    ample = vp[2];
    alt = vp[3];
#endif

    glClear(GL_COLOR_BUFFER_BIT);

    drawWays();
}

void GLWidget::modelTransform(glm::vec3 centre, float angle, float escala)
{
    glm::mat4 transform(1.0f);
    transform = glm::translate(transform, centre);
    transform = glm::scale(transform, glm::vec3(escala, escala, 1));

    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void GLWidget::projectTransform()
{
    glm::mat4 proj = glm::ortho(boundaries[0], boundaries[2], boundaries[1], boundaries[3], 0.0f, 15.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
}

void GLWidget::resizeGL(int w, int h)
{
    vpRatio = float(w) / float(h);
    boundaries[0] = sceneCenter[0] - zoomLevel * vpRatio;
    boundaries[2] = sceneCenter[0] + zoomLevel * vpRatio;
    boundaries[1] = sceneCenter[1] - zoomLevel;
    boundaries[3] = sceneCenter[1] + zoomLevel;
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    makeCurrent();
    if (event->type() == QEvent::KeyPress)
    {
        switch (event->key())
        {
        case Qt::Key_Plus:
            zoomLevel /= 1.2;
            movement = zoomLevel / 8;
            boundaries[0] = sceneCenter[0] - zoomLevel * vpRatio;
            boundaries[2] = sceneCenter[0] + zoomLevel * vpRatio;
            boundaries[1] = sceneCenter[1] - zoomLevel;
            boundaries[3] = sceneCenter[1] + zoomLevel;
            break;
        case Qt::Key_Minus:
            if (sceneCenter[0] - zoomLevel * vpRatio > data.getBoundaries()[0] and
                sceneCenter[0] + zoomLevel * vpRatio < data.getBoundaries()[2] and
                sceneCenter[1] - zoomLevel > data.getBoundaries()[1] and
                sceneCenter[1] + zoomLevel < data.getBoundaries()[3])
            {
                zoomLevel *= 1.2;
                movement = zoomLevel / 8;
                boundaries[0] = sceneCenter[0] - zoomLevel * vpRatio;
                boundaries[2] = sceneCenter[0] + zoomLevel * vpRatio;
                boundaries[1] = sceneCenter[1] - zoomLevel;
                boundaries[3] = sceneCenter[1] + zoomLevel;
            }
            break;
        case Qt::Key_A:
            if (sceneCenter[0] - movement - zoomLevel * vpRatio > data.getBoundaries()[0])
            {
                sceneCenter[0] -= movement;
                boundaries[0] = sceneCenter[0] - zoomLevel * vpRatio;
                boundaries[2] = sceneCenter[0] + zoomLevel * vpRatio;
            }
            break;
        case Qt::Key_S:
            if (sceneCenter[1] - movement - zoomLevel > data.getBoundaries()[1])
            {
                sceneCenter[1] -= movement;
                boundaries[1] = sceneCenter[1] - zoomLevel;
                boundaries[3] = sceneCenter[1] + zoomLevel;
            }
            break;
        case Qt::Key_D:
            if (sceneCenter[0] + movement + zoomLevel < data.getBoundaries()[2])
            {
                sceneCenter[0] += movement;
                boundaries[0] = sceneCenter[0] - zoomLevel * vpRatio;
                boundaries[2] = sceneCenter[0] + zoomLevel * vpRatio;
            }
            break;
        case Qt::Key_W:
            if (sceneCenter[1] + movement + zoomLevel * vpRatio < data.getBoundaries()[3])
            {
                sceneCenter[1] += movement;
                boundaries[1] = sceneCenter[1] - zoomLevel;
                boundaries[3] = sceneCenter[1] + zoomLevel;
            }
            break;
        case Qt::Key_R:
            boundaries = data.getBoundaries();
            break;
        default:
            event->ignore();
            break;
        }
        update();
    }
}

void GLWidget::createBuffers()
{
    for (uint i = 0; i < vaos.size(); ++i)
    {
        vector<glm::vec3> way = data.getWay(i);
        vector<glm::vec3> wayColor(way.size(), {1, 1, 1});

        glGenVertexArrays(1, &vaos[i]);
        glBindVertexArray(vaos[i]);

        GLuint VBO[2];
        glGenBuffers(2, &VBO[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, way.size() * sizeof(glm::vec3), &way[0], GL_STATIC_DRAW);
        glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(vertexLoc);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, way.size() * sizeof(glm::vec3), &wayColor[0],
                     GL_STATIC_DRAW);
        glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(colorLoc);

        glBindVertexArray(0);
    }
}

void GLWidget::loadShaders()
{
    QOpenGLShader fs(QOpenGLShader::Fragment, this);
    QOpenGLShader vs(QOpenGLShader::Vertex, this);

    fs.compileSourceFile("shaders/basicShader.frag");
    vs.compileSourceFile("shaders/basicShader.vert");

    program = new QOpenGLShaderProgram(this);
    program->addShader(&fs);
    program->addShader(&vs);
    program->link();
    program->bind();

    vertexLoc = glGetAttribLocation(program->programId(), "vertex");
    colorLoc = glGetAttribLocation(program->programId(), "color");

    transLoc = glGetUniformLocation(program->programId(), "TG");
    projLoc = glGetUniformLocation(program->programId(), "PROJ");
    canviLoc = glGetUniformLocation(program->programId(), "canviColor");
}
