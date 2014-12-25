#include "drawfence.h"
#include "GL/glu.h"

DrawFence::DrawFence()
{
}

DrawFence::~DrawFence()
{}

/*!
  Конструктор по координатам угла
*/
DrawFence::DrawFence(float nx1, float ny1, float nx2, float ny2)
{
    x1 = nx1;
    x2 = nx2;
    y1 = ny1;
    y2 = ny2;
}

/*!
  Рисует с заданным масштабом
*/
void DrawFence::draw(GLfloat scale)
{
    if (this->list == 0 || scale == 0) return;
    glBindTexture (GL_TEXTURE_2D, texture) ;
    glPushMatrix();
    glScalef(scale, scale, scale);
    glCallList(list);
    glPopMatrix();
}

/*!
  Рисует в точке с заданным масштабом
*/
void DrawFence::draw(GLfloat x, GLfloat y, GLfloat z, GLfloat scale)
{
    if (this->list == 0) return;
    glPushMatrix();
    glTranslatef(x,y,z);
    draw(scale);
    glPopMatrix();
}

/*!
  Загружает текстуры и создает список отображения
*/
void DrawFence::compile()
{
    QImage image;
    list = glGenLists(1);

    glGenTextures (1, &texture) ;
    image = loadImage("textures/wood3.jpg");
    glBindTexture (GL_TEXTURE_2D, texture) ;
    gluBuild2DMipmaps( GL_TEXTURE_2D,
                                       GL_RGBA,
                                       image.width(),
                                       image.height(),
                                       GL_RGBA,
                                       GL_UNSIGNED_BYTE,
                                       image.constBits());

    glGenTextures (1, &texture2) ;
    image = loadImage("textures/wood3.jpg");
    glBindTexture (GL_TEXTURE_2D, texture2);
    gluBuild2DMipmaps( GL_TEXTURE_2D,
                                       GL_RGBA,
                                       image.width(),
                                       image.height(),
                                       GL_RGBA,
                                       GL_UNSIGNED_BYTE,
                                       image.constBits());
    glNewList(list, GL_COMPILE);
    {
        DrawFenceLine(x1, y1, x2, y1, 1.7);

        DrawFenceLine(x2, y1, x2, y2, 1.7);

        DrawFenceLine(x2, y2, x1, y2, 1.7);

        DrawFenceLine(x1, y2, x1, y1, 1.7);
    }
    glEndList();
}


/*!
  Отрисовка одной стенки частокола. Параметр whitespacesize задает расстояние между соседними столбами.
  Технически можно вставить 3ds модель изгороди.
  */
void DrawFence::DrawFenceLine(float x1, float y1, float x2, float y2, float z)
{
    float dx = (x2 - x1);
    float dy = (y2 - y1);
    float linelength = (float) sqrt(dx*dx + dy*dy);
    float itemsize = 0.1, whitespacesize = 0;
    float itempos = 0;

    GLUquadric *q = gluNewQuadric();
    gluQuadricTexture(q, GL_TRUE);
    glPushMatrix();
    glTranslatef(x1,y1,0);
    while (itempos + itemsize < linelength){

        float mx1 = itempos/linelength*dx;
        float my1 = itempos/linelength*dy;
        glPushMatrix();

        glTranslatef(mx1, my1, 0);

        glBindTexture (GL_TEXTURE_2D, texture) ;
        gluCylinder(q, itemsize, itemsize, z, 16, 4);

        glTranslatef(0, 0, z);
        glBindTexture (GL_TEXTURE_2D, texture2) ;
        gluCylinder(q, itemsize, 0, z*0.05, 16, 4);

        glPopMatrix();

        itempos += itemsize*2;
    }
    glPopMatrix();
}
