#include "drawsnow.h"
#include "GL/glu.h"

/*!
Инициализирует параметры снега, создает необходимое количество снежинок, назначая каждой случайные параметры.
*/
drawsnow::drawsnow(int snowflakes_count, int sizeX,int sizeY)
{
    H = sizeX;
    W = sizeY;
    QImage image = loadImage("textures/sun.jpg");

    glGenTextures (1, &texture) ;

    glBindTexture (GL_TEXTURE_2D, texture) ;
    gluBuild2DMipmaps( GL_TEXTURE_2D,
                                       GL_RGBA,
                                       image.width(),
                                       image.height(),
                                       GL_RGBA,
                                       GL_UNSIGNED_BYTE,
                                       image.constBits());


    for (int i = 0; i < snowflakes_count; i++){
        snow4f newflake;
        newflake.x = (qrand()%int(H))-H/2.0f;
        newflake.y = (qrand()%int(W))-W/2.0f;
        newflake.z = (qrand()%100);
        newflake.vx = 0;
        newflake.vy = 0;
        newflake.vz = -1;
        newflake.scale = maxsize*(qrand()%1000)/1000.0f;
        snowflakes.append(newflake);
    }
}

/*!
Расчет нового положения снежинок на основе данных об их скорости.
*/
void drawsnow::animate(){
    for (QList<snow4f>::iterator newflake = snowflakes.begin(); newflake < snowflakes.end(); newflake++){
        newflake->vx += 0.3*(500 - qrand()%1000)/1000.0;
        newflake->vy += 0.3*(500 - qrand()%1000)/1000.0;
        newflake->vz += 0.03*(500 - qrand()%1000)/1000.0;
        newflake->x += newflake->vx;
        newflake->y += newflake->vy;
        newflake->z += newflake->vz;
        if (newflake->z < 0) newflake->z = 100;
        if (fabs(newflake->x) > 3*H) newflake->x *= -1;
        if (fabs(newflake->y) > 3*W) newflake->y *= -1;
    }
}

/*!
  Отрисовка снежинок. Циклом по списку.
  Технически в этом месте вместо gluSphere можно вставить любой другой примитив или загрузить 3DS модель.
*/
void drawsnow::draw(){
    GLUquadric * q = gluNewQuadric();
    glBindTexture (GL_TEXTURE_2D, texture) ;
    glColor3ub(255,255,255);
    for (QList<snow4f>::iterator flake = snowflakes.begin(); flake < snowflakes.end(); flake++){
        snow4f curfl = *flake;
        glPushMatrix();
        glTranslatef(curfl.x,curfl.y,curfl.z);
        gluSphere(q,curfl.scale,4,4);
        glPopMatrix();
    }
}
