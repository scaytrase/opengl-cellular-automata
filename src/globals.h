#ifndef GLOBALS_H
#define GLOBALS_H
#include "GL/gl.h"
#include <qmath.h>

constexpr static GLfloat g_nearPlane =  0.5f; //!< Ближняя плоскость отсечения
constexpr static GLfloat g_farPlane  = 1000.0f; //!< Дальняя плоскость отсечения

static int g_Width; //!< Начальная ширина окна
static int g_Height; //!< Начальная высота окна

float
		lu=-0.26f,lv=0.73f, skyRadius=100.0f;
float
		g_lightPos_0 [4] = {(float) (skyRadius*qCos(lu)*qCos(lv)), (float) (skyRadius*qSin(lu)*qCos(lv)), (float) (skyRadius*qSin(lv)), 1 },     //!< Позиция первого источника света
		g_lightPos_1 [4] = { 50.0f, 50.0f, 50.0f, 1.0f },   //!< Позиция второго источника света
		g_lightSpot_0[4] = { 0.0f, 0.0f, -1.0f, 1.0f },     //!< Направление первого источника света
		g_lightSpot_1[4] = { -1.0f, -1.0f, 0.0f, 1.0f },    //!< Направление второго источника света
		g_lightAmb_0 [4] = { 0.2, 0.2, 0.2, 0 },//!< Амбиент составляющаяя первого источника света
		g_lightAmb_1 [4] = { 0.2, 0.2, 0.2, 0 },//!< Амбиент составляющаяя второго источника света
		g_lightDif_0 [4] = { 0.9, 0.9, 0.9, 1 },//!< Диффузная составляющаяя первого источника света
		g_lightDif_1 [4] = { 0.9, 0.9, 0.9, 1 },//!< Диффузная составляющаяя второго источника света
		g_lightSpe_0 [4] = { 0.9, 0.9, 0.9, 1 },//!< Бликующая составляющаяя первого источника света
		g_lightSpe_1 [4] = { 0.9, 0.9, 0.9, 1 };//!< Бликующая составляющаяя второго источника света


static float
		eye_x, //!< Положение наблюдателя X
		eye_y, //!< Положение наблюдателя Y
		eye_z; //!< Положение наблюдателя Z

static float
		direction_h, //!< Угол наблюдателя по горизонтали
		direction_v; //!< Угол наблюдателя по вертикали
constexpr static float
		step = 0.2f; //!< Длина виртуального шага


#endif // GLOBALS_H
