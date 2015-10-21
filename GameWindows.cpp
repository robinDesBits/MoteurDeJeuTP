#include "GameWindows.h"

GameWindow::GameWindow()
    : m_program(0)
    , m_frame(0)
{
    ChargerMap();

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
GLuint GameWindow::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}
void GameWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

   // glMatrixMode(GL_MODELVIEW);
}

void GameWindow:: ChargerMap(QString localPath)
{
    int nbrSommetCote=240;
    if (QFile::exists(localPath))
    {
       m_image = QImage(localPath);
    }
    QRgb pixel;

    //Creation des Points
    for(int i=0;i<172800;i+=3)
    {
      pixel = m_image.pixel(i,j);

      points[i]=((i/3%240)-50)*0.02f-0.5f;
      points[i+1]=((i/720))*(-0.01f)+1;
      if(i%240==0 || i<720 || i>172080 || i/3%240==239)
      {
        points[i+2]=0;
      }
      else
      {
        points[i+2]= rand()/(float)RAND_MAX * (0.1f);
      }
    }
}

void GameWindow::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, avancerZ);
    matrix.rotate(rotX, 1, 0, 0);
    matrix.rotate(rotY, 0, 1, 0);
    matrix.rotate(rotZ, 0, 0, 1);



    m_program->setUniformValue(m_matrixUniform, matrix);

    //creation des couleurs
    GLfloat colors[172800];
    for(int i=0;i<768;i+=3)
    {
        colors[i]=1.0f;
        colors[i+1]=0.0f;
        colors[i+2]=0.0f;
    }


    //Affichage des points
    GLfloat vertices[1440];
    int i=0;
    for(int j=0;j<239;j++)
    {
         for(i=0;i<1440;i+=3)
         {
              if((i/2)%3==0)
              {
                vertices[i]=points[i/2+j*720];
                vertices[i+1]=points[(i/2+1)+j*720];
                vertices[i+2]=points[i/2+2+j*720];
              }
              else
              {
                  vertices[i]=points[i-(2*(i-2)/4)+717+j*720];
                  vertices[i+1]=points[i-(2*(i-2)/4)+717+1+j*720];
                  vertices[i+2]=points[i-(2*(i-2)/4)+717+2+j*720];
              }
          }
         glPolygonMode(GL_FRONT_AND_BACK,  GL_LINE);

         glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
         glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

         glEnableVertexAttribArray(0);
         glEnableVertexAttribArray(1);
         //glEnableVertexAttribArray(1);
         glDrawArrays(GL_TRIANGLE_STRIP, 0, 480);
         glDisableVertexAttribArray(1);

         glDisableVertexAttribArray(0);
         //glDisableVertexAttribArray(0);
    }
    ++m_frame;
}

void GameWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case 'Z':
        avancerZ+= 0.10f;
        break;
    case 'S':
        avancerZ -= 0.10f;
        break;
    case 'A':
        rotX += 1.0f;
        break;
    case 'E':
        rotX -= 1.0f;
        break;
    case 'Q':
        rotY += 1.0f;
        break;
    case 'D':
        rotY -= 1.0f;
        break;
    }
    renderNow();
}
