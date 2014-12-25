#ifndef DRAWFENCE_H
#define DRAWFENCE_H
#include "GL/gl.h"
#include "drawobject.h"
#include "textureloader.h"
#include <math.h>

//! Класс для отрисовки частокола

/*!
  Класс инициализируется координатоми углов поля. Отрисовывает плотный или разреженный частокол в зависимости от внутреннего параметра
*/
class DrawFence : public DrawObject
{
private:
    //! Прекомпиленный список частокола
    GLuint list;
    //! Текстура
    GLuint texture,
    //! Текстура
    texture2;
    float
            x1,
            x2,
            y1,
            y2;
    //! Отрисовка одной линии частокола
    void DrawFenceLine(float x1, float y1, float x2, float y2, float z);
public:
    //! Конструктор
    DrawFence();
    //! Констурктор
    DrawFence(float nx1, float ny1, float nx2, float ny2);
    //! Деструктор
    ~DrawFence();

    //! Отрисовка
    virtual void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat scale);
    //! Отрисовка
    virtual void draw(GLfloat scale);
    //! Прекомпилирование списка
    virtual void compile();
};

#endif // DRAWFENCE_H
