#include "GameWindows.h"

GameWindow::GameWindow()
    : m_program(0)
    , m_frame(0)
{
    ChargerMap("./heightmap-1.png");

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

   //glMatrixMode(GL_MODELVIEW);
}

void GameWindow:: ChargerMap(QString localPath)
{
    if (QFile::exists(localPath))
    {
       m_image = QImage(localPath);
    }
    QRgb pixel;
    hauteur=m_image.height();
    largeur=m_image.width();
    int id;
    points=new GLfloat[m_image.width()*m_image.height()*3];
    //Creation des Points
    couleurs= new GLfloat[m_image.width()*m_image.height()*3+6*m_image.height()];

    for(int i = 0; i < m_image.width()*3; i+=3)
    {
        for(int j = 0; j < m_image.height()*3; j+=3)
        {
            id = j*m_image.width() +i;
            pixel = m_image.pixel(i/3,j/3);
            points[id]= (float)i/(m_image.width()) - ((float)m_image.width()/2.0)/m_image.width()-1;
            points[id+1] = (float)j/(m_image.height()) - ((float)m_image.height()/2.0)/m_image.height();
            points[id+2] = 0.001f * (float)(qRed(pixel));
            couleurs[id]=1.0f;
            couleurs[id+1]=0.0f;
            couleurs[id+2]=0.0f;
        }
    }
   vertices= new GLfloat[m_image.width()*m_image.height()*3+6*m_image.height()];

    int i=0;
    int j=0;
    for(j = 0; j <  m_image.height(); j++)
    {
        for(i = 0; i < m_image.width()*6; i+=3)
        {
             if((i/2)%3==0)
             {
               vertices[j*2*m_image.width()+i]=points[i/2+j*largeur];
               vertices[j*2*m_image.width()+i+1]=points[(i/2+1)+j*largeur];
               vertices[j*2*m_image.width()+i+2]=points[i/2+2+j*largeur];
             }
             else
             {
                 vertices[j*2*m_image.width()+i]=points[(i/3/2)*3+j*largeur+(j+1)*largeur];
                 vertices[j*2*m_image.width()+i+1]=points[(i/3/2)*3+j*largeur+(j+1)*largeur+1];
                 vertices[j*2*m_image.width()+i+2]=points[(i/3/2)*3+j*largeur+(j+1)*largeur+2];
             }
             /*couleurs[j*2*m_image.width()+i]=1.0f;
             couleurs[j*2*m_image.width()+i+1]=0.0f;
             couleurs[j*2*m_image.width()+i+2]=0.0f;*/
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
    matrix.perspective(80.0f, 4.0f/3.0f, 0.1f, 100.0f);
    matrix.translate(0, 0, avancerZ);
    matrix.rotate(rotX, 1, 0, 0);
    matrix.rotate(rotY, 0, 1, 0);
    matrix.rotate(rotZ, 0, 0, 1);



    m_program->setUniformValue(m_matrixUniform, matrix);


    //Affichage des points

         glPolygonMode(GL_FRONT_AND_BACK,  GL_LINE);

         glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, points);
         glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, couleurs);

         glEnableVertexAttribArray(0);
         glEnableVertexAttribArray(1);

         glDrawArrays(GL_POINTS, 0, largeur*hauteur);

         glDisableVertexAttribArray(1);
         glDisableVertexAttribArray(0);


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
