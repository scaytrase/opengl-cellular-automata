#include "draw3ds.h"
#include "GL/glu.h"

Draw3DS::Draw3DS() {
    this->model = new L3DS();
    //this->list = 0;
}

void Draw3DS::LoadTextures(QString tr, QString leav) {
    QImage image = loadImage(tr.toUtf8());

    glGenTextures(1, &trunk);

    glBindTexture(GL_TEXTURE_2D, trunk);
    gluBuild2DMipmaps(GL_TEXTURE_2D,
            GL_RGBA,
            image.width(),
            image.height(),
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image.constBits());
    image = loadImage(leav.toUtf8());

    glGenTextures(1, &leaves);

    glBindTexture(GL_TEXTURE_2D, leaves);
    gluBuild2DMipmaps(GL_TEXTURE_2D,
            GL_RGBA,
            image.width(),
            image.height(),
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image.constBits());

}

Draw3DS::~Draw3DS() {
    delete model;
}

bool Draw3DS::LoadFromFile(const char *filename) {
    return (*model).LoadFile(filename);
}

void Draw3DS::draw(GLfloat scale) {
    //if (this->list == 0) return;
    glPushMatrix();
    glScalef(scale, scale, scale);

    glBindTexture(GL_TEXTURE_2D, leaves);
    glCallList(leaf_list);
    glBindTexture(GL_TEXTURE_2D, trunk);
    glCallList(trunk_list);
    glPopMatrix();
}

void Draw3DS::draw(GLfloat x, GLfloat y, GLfloat z, GLfloat scale) {
    //if (this->list == 0) return;
    glPushMatrix();
    glTranslatef(x, y, z);
    draw(scale);
    glPopMatrix();
}

void Draw3DS::compile() {
    leaf_list = glGenLists(2);
    trunk_list = leaf_list + 1;

    int MeshCount = (*model).GetMeshCount();

    glNewList(leaf_list, GL_COMPILE);
    {
        glBegin(GL_TRIANGLES);
        {
            for (int i = 1; i < MeshCount; i += 2) {
                LMesh *mesh = (*model).GetMesh(i);
                if (mesh) {

                    int FaceCount = (*mesh).GetFaceCount();
                    for (int j = 0; j < FaceCount; j++) {
                        LFace face = (*mesh).GetFace(j);
                        glNormal3f(face.normal.x, face.normal.y, face.normal.z);
                        glTexCoord2f(face.uv[0].u, face.uv[0].v);
                        glVertex3f(face.vertices[0].x, face.vertices[0].y, face.vertices[0].z);
                        glTexCoord2f(face.uv[1].u, face.uv[1].v);
                        glVertex3f(face.vertices[1].x, face.vertices[1].y, face.vertices[1].z);
                        glTexCoord2f(face.uv[2].u, face.uv[2].v);
                        glVertex3f(face.vertices[2].x, face.vertices[2].y, face.vertices[2].z);
                    }
                }
            }
        }
        glEnd();
    }
    glEndList();
    glNewList(trunk_list, GL_COMPILE);
    {
        glBegin(GL_TRIANGLES);
        {
            for (int i = 0; i < MeshCount; i += 2) {
                LMesh *mesh = (*model).GetMesh(i);
                if (mesh) {

                    int FaceCount = (*mesh).GetFaceCount();
                    for (int j = 0; j < FaceCount; j++) {
                        LFace face = (*mesh).GetFace(j);
                        glNormal3f(face.normal.x, face.normal.y, face.normal.z);
                        glTexCoord2f(face.uv[0].u, face.uv[0].v);
                        glVertex3f(face.vertices[0].x, face.vertices[0].y, face.vertices[0].z);
                        glTexCoord2f(face.uv[1].u, face.uv[1].v);
                        glVertex3f(face.vertices[1].x, face.vertices[1].y, face.vertices[1].z);
                        glTexCoord2f(face.uv[2].u, face.uv[2].v);
                        glVertex3f(face.vertices[2].x, face.vertices[2].y, face.vertices[2].z);
                    }
                }
            }
        }
        glEnd();
    }
    glEndList();
}
