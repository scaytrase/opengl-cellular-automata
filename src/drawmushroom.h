#ifndef DRAWMUSHROOM_H
#define DRAWMUSHROOM_H
#include "drawobject.h"

//! Класс модели гриба

/*!
  Предоставляет инструмент манипулирования феерическими грибами
*/
class DrawMushroom : public DrawObject
{
private:
    //! Список содержащий модель гриба
    GLuint list;
    //! Полная высота гриба
    constexpr static float mushroom_height = 0.6f;
    //! Радиус шляпки гриба
    constexpr static float mushroom_hat_rad = 0.25f;
    //! Радиус нижней части ножки
    constexpr static float mushroom_thickness_bottom = 0.1f;
    //! Радиус верхней части ножки
    constexpr static float mushroom_thickness_top = 0.05f;
    GLuint
            //! Текстура ножки гриба
            texbody,
            //! Текстура шляпки гриба
            texhat;
public:
    //! Конструктор
    DrawMushroom();
    //! Деструктор
    ~DrawMushroom();
    //! Рисует гриб в заданной позиции,с заданным масштабом. относительно текущего
    virtual void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat scale);
    //! Перегруженная функция. Рисует гриб с заданным масштабом
    virtual void draw(GLfloat scale);
    //! Создает список для гриба
    virtual void compile();
};

#endif // DRAWMUSHROOM_H
