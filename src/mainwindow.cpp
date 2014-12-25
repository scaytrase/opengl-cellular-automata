#include <iostream>
#include "mainwindow.h"
#include "GL/glu.h"
#include "glmushroom_model.h"
#include "draw3ds.h"
#include "drawmushroom.h"
#include "drawfence.h"
#include "drawsnow.h"
#include "ColorCalculator.h"


//! Идет ли анимация
bool started = true;

//! Модель
static glmushroom_logger main_logger("mushroom.log");
//! Вспомогательный класс для отчетов
static glmushroom_model main_model(&main_logger);

//! Текущие данные
static QVector<QVector<float> > data;
//! Таймеры приложения
static QTime timer1, timer2;
float fps = 0, framerate = 0;
//static Model_3DS TreeObject;

//! Дерево
static Draw3DS *TreeObject;
//! Гриб
static DrawMushroom *Mushroom;
//! Изгородь
static DrawFence *Fence;
//! Снег
static drawsnow *snow;
//! Список отображения земли
static GLuint plane;

#include "globals.h"

void compileObjects();

void shadowmatrix(float matrix[4][4],
        float plane[4],
        float lightpos[4]);

void reflectionmatrix(float reflection_matrix[4][4],
        float plane_point[3],
        float plane_normal[3]);

void RenderShadows(void);

void RenderGround(void);

void RenderObjects(void);

void RenderEnvy(void);

void reshape(GLint width, GLint height);

void InitGraphics();

void MouseMotion(int x, int y);

void AnimateScene();

void LoadObjects();

void LoadTextures();

void Keyboard(unsigned char key, int x, int y);

int subcount = 0;

float CellSize = 3;
float FieldWidth = main_model.sizeX();
float FieldHeight = main_model.sizeY();
float FieldBorder = 0;

#define NUM_TEXTURES 10
GLuint TextureIDs[NUM_TEXTURES];
GLuint DepthTex;
QImage texture;


MainWindow::MainWindow(QWindow *parent)
        : QWindow(parent), m_update_pending(false), m_animating(false), m_context(0), m_device(0) {
    setSurfaceType(QWindow::OpenGLSurface);


}

void MainWindow::initialize() {
    data = main_model.get_data();

    g_Width = 100;
    g_Height = 100;

    eye_x = 0; //!< Положение наблюдателя X
    eye_y = 0; //!< Положение наблюдателя Y
    eye_z = 2; //!< Положение наблюдателя Z

    direction_h = 0; //!< Угол наблюдателя по горизонтали
    direction_v = 0; //!< Угол наблюдателя по вертикали

    LoadObjects();
    compileObjects();
    LoadTextures();
    InitGraphics();
    timer1.start();
    timer2.start();


}

void MainWindow::render() {
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size());

    if (!started) return;
    if (timer2.elapsed() > 1) {
        if (subcount < 100)
        {
            main_model.send_Event(EVENT_SUBTICK);
            subcount++;
        } else {
            main_model.send_Event(EVENT_TICK);
            subcount = 0;
        }
        std::cout << "Frame " << subcount << std::endl;
        data = main_model.get_data();
        timer2.restart();
    }
    (*snow).animate();

    glLoadIdentity();
    gluLookAt(eye_x,
            eye_y,
            eye_z,
            eye_x + 1*qCos(direction_h)*qCos(direction_v),
            eye_y + 1*qSin(direction_h)*qCos(direction_v),
            eye_z + 1*qSin(direction_v),
            0,
            0,
            1);

    // Render the scene
    glClearColor(0.0f,0.01f * subcount,0.0f,1.0f);

    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos_0);
    glLightfv(GL_LIGHT1, GL_POSITION, g_lightPos_1);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, g_lightSpot_0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, g_lightSpot_1);

    // Render Objects
    RenderObjects();
    // Render Envy
    RenderEnvy();
    // Render World
    RenderGround();
    // Render Shadows
    RenderShadows();
    // Render Mirror

    renderLater();
}

void MainWindow::renderLater() {
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }


}

bool MainWindow::event(QEvent *event) {
    switch (event->type()) {
        case QEvent::UpdateRequest:
            m_update_pending = false;
            renderNow();
            return true;
        default:
            return QWindow::event(event);
    }
}

void MainWindow::exposeEvent(QExposeEvent *event) {
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void MainWindow::renderNow() {
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void MainWindow::setAnimating(bool animating) {
    m_animating = animating;

    if (animating)
        renderLater();
}

void LoadTextures() {

    glGenTextures(NUM_TEXTURES, TextureIDs);


    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
            GL_MODULATE);

    texture = loadImage("textures/tree_hat.jpg");
    glBindTexture(GL_TEXTURE_2D, TextureIDs[0]);
    gluBuild2DMipmaps(GL_TEXTURE_2D,
            GL_RGBA,
            texture.width(),
            texture.height(),
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            texture.constBits());

    texture = loadImage("textures/tree_hat.jpg");
    glBindTexture(GL_TEXTURE_2D, TextureIDs[1]);
    gluBuild2DMipmaps(GL_TEXTURE_2D,
            GL_RGBA,
            texture.width(),
            texture.height(),
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            texture.constBits());

    texture = loadImage("textures/grass3.jpg");
    glBindTexture(GL_TEXTURE_2D, TextureIDs[2]);
    gluBuild2DMipmaps(GL_TEXTURE_2D,
            GL_RGBA,
            texture.width(),
            texture.height(),
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            texture.constBits());

    texture = loadImage("textures/sun.jpg");
    glBindTexture(GL_TEXTURE_2D, TextureIDs[3]);
    gluBuild2DMipmaps(GL_TEXTURE_2D,
            GL_RGBA,
            texture.width(),
            texture.height(),
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            texture.constBits());
    (*TreeObject).LoadTextures("textures/wood3.jpg", "textures/grass3.jpg");

    glTexParameteri(GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER, GL_NEAREST);

}

//! Загрузка обьектов

/*! Используется смесь обьектов 3DS и OpenGL*/
void LoadObjects() {
    Mushroom = new DrawMushroom();//3DS();
    TreeObject = new Draw3DS();
    (*TreeObject).LoadFromFile("models/M_TREE3.3DS");

    Fence = new DrawFence(-CellSize * FieldWidth / 2.0f,
            -CellSize * FieldHeight / 2.0f,
            CellSize * FieldWidth / 2.0f,
            CellSize * FieldHeight / 2.0f);

    snow = new drawsnow(1000, (int) (2 * FieldHeight), (int) (2 * FieldWidth));
}

//! Инициализация графики

void InitGraphics() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_TEXTURE_2D);        // Разрешение наложение текстуры

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);

//    glEnable(GL_LIGHTING);

//    glEnable(GL_LIGHT0);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, g_lightAmb_0);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, g_lightDif_0);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, g_lightSpe_0);
//    glLightfv(GL_LIGHT0, GL_POSITION, g_lightPos_0);
//    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, g_lightSpot_0);

}

//! Рендер теней

/*! Способом из методички*/
void RenderShadows() {
    glPushMatrix();

    float floorShadow_l0[4][4];
    float floorShadow_l1[4][4];
    float floor[4] = {0, 0, 1, -0.001f};
    shadowmatrix(floorShadow_l0, floor, g_lightPos_0);
    shadowmatrix(floorShadow_l1, floor, g_lightPos_1);


    glEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_ALWAYS, 2, 2);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    RenderGround();
    glStencilFunc(GL_EQUAL, 2, 2);

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glMultMatrixf((GLfloat *) floorShadow_l0);

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glColor4f(0, 0, 0, 0.5);
    RenderObjects();
    (*Fence).draw(1);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glDisable(GL_STENCIL_TEST);

    glPopMatrix();
}
//! Отрисовка земли

/*! Отрисовка плоскости земли с назачением текстуры*/
void RenderGround() {
    glPushMatrix();

    glColor3ub(1, 1, 1);
    glScalef(500, 500, 1);
    glBindTexture(GL_TEXTURE_2D, TextureIDs[0]);
    glCallList(plane);

    glPopMatrix();

}

//! Отрисовка декораций

/*! Солнце, забор, небо, прочие рюшки*/
void RenderEnvy() {
    GLUquadric *q = gluNewQuadric();
    gluQuadricTexture(q, GL_TRUE);

    glBindTexture(GL_TEXTURE_2D, TextureIDs[2]);

    glPushMatrix();

    glScalef(-1, -1, -1);

    glColor3f(1, 1, 1);
    glDisable(GL_LIGHTING);
    gluSphere(q, skyRadius, 30, 30);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glPushMatrix();

    glTranslatef(g_lightPos_0[0], g_lightPos_0[1], g_lightPos_0[2]);;
    glBindTexture(GL_TEXTURE_2D, TextureIDs[3]);

    glDisable(GL_LIGHTING);

    glColor4f(1, 1, 1, 0.1);
    gluSphere(q, 3, 16, 16);

    glEnable(GL_LIGHTING);

    glPopMatrix();

    (*Fence).draw(1);
    (*snow).draw();

}


//! Отрисовка деревьев и грибов

/*! Главное, что надо было нарисовать*/
void RenderObjects(void) {
    float MushroomSize;

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();

    glTranslatef(-FieldWidth * CellSize / 2.0f, -FieldHeight * CellSize / 2.0f, 0);

    for (int x = 0; x < FieldWidth; x++)
        for (int y = 0; y < FieldHeight; y++) {
            if (data[y][x] == 1) {
                (*TreeObject).draw(x * CellSize, y * CellSize, 0, 1);
            } else if (data[y][x] == 0) {;
            } else if (data[y][x] < 0) {
                MushroomSize = GetMushroomScale(data[y][x]);
                (*Mushroom).draw(x * CellSize, y * CellSize, 0, MushroomSize);
            }
        }

    glPopMatrix();

}

//! Вычисление матрицы проекции для  текущего положения света и земли

/*! Из той же методички*/
void shadowmatrix(float matrix[4][4], //!< Результат работы
        float plane[4], //!< Плоскость пола
        float lightpos[4]) { //!< Положение света
    float dot;
    dot = plane[0] * lightpos[0] +
            plane[1] * lightpos[1] +
            plane[2] * lightpos[2] +
            plane[3] * lightpos[3];

    matrix[0][0] = dot - lightpos[0] * plane[0];
    matrix[1][0] = 0.0f - lightpos[0] * plane[1];
    matrix[2][0] = 0.0f - lightpos[0] * plane[2];
    matrix[3][0] = 0.0f - lightpos[0] * plane[3];

    matrix[0][1] = 0.0f - lightpos[1] * plane[0];
    matrix[1][1] = dot - lightpos[1] * plane[1];
    matrix[2][1] = 0.0f - lightpos[1] * plane[2];
    matrix[3][1] = 0.0f - lightpos[1] * plane[3];

    matrix[0][2] = 0.0f - lightpos[2] * plane[0];
    matrix[1][2] = 0.0f - lightpos[2] * plane[1];
    matrix[2][2] = dot - lightpos[2] * plane[2];
    matrix[3][2] = 0.0f - lightpos[2] * plane[3];

    matrix[0][3] = 0.0f - lightpos[3] * plane[0];
    matrix[1][3] = 0.0f - lightpos[3] * plane[1];
    matrix[2][3] = 0.0f - lightpos[3] * plane[2];
    matrix[3][3] = dot - lightpos[3] * plane[3];

}

//! Создание списков отображения

/*! Генерация обьектов плоскости, гриба, дерева и забора*/
void compileObjects() {
    plane = glGenLists(1);
    glNewList(plane, GL_COMPILE);
    {
        glBegin(GL_QUADS);
        {
            glTexCoord2f(0, 0);
            glVertex3f(-1.5f, -1.5f, 0.0f);
            glTexCoord2f(1000, 0);
            glVertex3f(1.5f, -1.5f, 0.0f);
            glTexCoord2f(1000, 1000);
            glVertex3f(1.5f, 1.5f, 0.0f);
            glTexCoord2f(0, 1000);
            glVertex3f(-1.5f, 1.5f, 0.0f);
        }
        glEnd();

    }
    glEndList();
    (*Mushroom).compile();
    (*TreeObject).compile();
    (*Fence).compile();
}
