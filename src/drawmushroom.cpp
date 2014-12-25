#include "drawmushroom.h"
#include "textureloader.h"

#include <QImage>
#include "GL/glu.h"

DrawMushroom::DrawMushroom()
{
}

DrawMushroom::~DrawMushroom()
{
}

void DrawMushroom::draw(GLfloat scale)
{
    if (this->list == 0 || scale == 0) return;
    glPushMatrix();
    glScalef(scale, scale, scale);
    glCallList(list);
    glPopMatrix();
}

void DrawMushroom::draw(GLfloat x, GLfloat y, GLfloat z, GLfloat scale)
{
    if (this->list == 0) return;
    glPushMatrix();
    glTranslatef(x,y,z);
    draw(scale);
    glPopMatrix();
}

void DrawMushroom::compile()
{
	QImage texture;
	glGenTextures (1, &texbody ) ;

	glGenTextures (1, &texhat ) ;

	texture = loadImage("textures/wood3.jpg");
	glBindTexture (GL_TEXTURE_2D, texbody) ;
	gluBuild2DMipmaps( GL_TEXTURE_2D,
					   GL_RGBA,
					   texture.width(),
					   texture.height(),
					   GL_RGBA,
					   GL_UNSIGNED_BYTE,
					   texture.constBits());

	texture = loadImage("textures/mushroom_hat.jpg");
	glBindTexture (GL_TEXTURE_2D, texhat) ;
	gluBuild2DMipmaps( GL_TEXTURE_2D,
					   GL_RGBA,
					   texture.width(),
					   texture.height(),
					   GL_RGBA,
					   GL_UNSIGNED_BYTE,
					   texture.constBits());

	list = glGenLists(1);


    glNewList(list, GL_COMPILE);
    {
        GLUquadricObj * quadratic = gluNewQuadric();
        gluQuadricTexture ( quadratic , GL_TRUE);

        glPushMatrix();
        glTranslatef(0,0,mushroom_height-mushroom_hat_rad);

        glBindTexture (GL_TEXTURE_2D, texhat) ;
        gluCylinder(quadratic,
                    mushroom_hat_rad,
                    mushroom_hat_rad*0.8,
                    (mushroom_height-mushroom_hat_rad)/4.0f,
                    16 ,
                    2);
        glTranslatef(0,0,(mushroom_height-mushroom_hat_rad)/4.0f);
        gluCylinder(quadratic,
                    mushroom_hat_rad*0.8f,
                    mushroom_hat_rad*0.6f,
                    (mushroom_height-mushroom_hat_rad)/4.0,
                    16 ,
                    2);
        glTranslatef(0,0,(mushroom_height-mushroom_hat_rad)/4.0f);
        gluCylinder(quadratic,
                    mushroom_hat_rad*0.6f,
                    mushroom_hat_rad*0.3f,
                    (mushroom_height-mushroom_hat_rad)/4.0,
                    16 ,
                    2);
        glTranslatef(0,0,(mushroom_height-mushroom_hat_rad)/4.0f);
        gluCylinder(quadratic,
                    mushroom_hat_rad*0.3f,
                    mushroom_hat_rad*0.0f,
                    (mushroom_height-mushroom_hat_rad)/16.0f,
                    16 ,
                    2);

        glPopMatrix();

        glBindTexture (GL_TEXTURE_2D, texbody) ;
        gluCylinder(quadratic,
                    mushroom_thickness_bottom,
                    mushroom_thickness_top,
                    mushroom_height-mushroom_hat_rad,
                    32 ,
                    32);
    }
    glEndList();
}
