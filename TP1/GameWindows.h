#ifndef GAMEWINDOWS_H
#define GAMEWINDOWS_H
#include "openglwindow.h"
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QKeyEvent>
#include <QtCore/qmath.h>


class GameWindow : public OpenGLWindow
{
public:
    GameWindow();

    void initialize();
    void render();
    void keyPressEvent(QKeyEvent *event);
    void ChargerMap(QString localPath);

private:

    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    QImage m_image;

    QOpenGLShaderProgram *m_program;
    int m_frame;

    GLfloat *points;
    GLfloat *couleurs;
    GLfloat *vertices;

    int rotX=-45;
    int rotY= 0;
    int rotZ= 0;
    int largeur;
    int hauteur;
    float deplacerX=-1;
    float avancerZ=0;
    float scale=0;
};

#endif // GAMEWINDOWS_H
