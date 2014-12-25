#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include "GL/gl.h"

//! Общий класс для объектов модели
/*!
  Содержит основную модель взаимодействия с графическими объектами.
*/
class DrawObject
{
public:
	/*! Конструктор */
	DrawObject();
	/*! Деструктор */
	virtual ~DrawObject() = 0;

	/*! Подготовка объекта */
	virtual void compile() = 0;
	/*! Вывод объекта в указанные координаты с указанным масштабированием */
	virtual void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat scale) = 0;
	/*! Вывод объекта в текущую точку отсчёта с указанным масшитабированием */
	virtual void draw(GLfloat scale) = 0;
};

#endif // DRAWOBJECT_H
