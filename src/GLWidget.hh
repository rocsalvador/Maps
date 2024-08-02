#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "Point.hh"
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include <stdio.h>
#include "Data.hh"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    virtual void initializeGL();

    virtual void paintGL();

    virtual void resizeGL(int width, int height);

    virtual void keyPressEvent(QKeyEvent *event);

private:
    void createBuffers();
    void loadShaders();
    void modelTransform(glm::vec3 centre, float angle, float escala);
    void projectTransform();
    void initValues();
    void initCamera();
    void drawWays();

    glm::vec3 sceneCenter;

    float vpRatio;
    float zoomLevel;
    float movement;

    Data data;

    glm::vec4 boundaries;

    GLuint vertexLoc, colorLoc;

    GLuint transLoc, canviLoc, canviColor, projLoc;

    vector<GLuint> vaos;

    QOpenGLShaderProgram *program;
};
