#ifndef DRAW3DS_H
#define DRAW3DS_H

#include "l3ds_generic/l3ds.h"
#include "drawobject.h"
#include "textureloader.h"

//! Класс 3DS Обьекта

/*!
  Предоставляет инструмент манипулирования обьектами в формате 3D Studio
*/
class Draw3DS : public DrawObject
{
private:
    GLuint
            trunk,//!< Текстура ствола и веток
            leaves;//!< Текстура листьев
    L3DS *model;//!< Ссылка на обьект
    GLuint
            trunk_list,//!< Прекомпиленный список, хранящий модель.
            leaf_list; //!< Листья
public:
    void LoadTextures(QString tr, QString leaves);
    Draw3DS(); //!< Конструктор Обьекта
    ~Draw3DS();//!< Деструктор Обьекта

    //! Функция загрузки модели из файла
    /*!
      @param filename - имя файла, содержащего 3Д модель
      @return true если загрузка успешной загрузки, false иначе.
    */
    bool LoadFromFile(const char *filename /*!< Имя файла*/ );
    //! Функция отрисовки модели
    /*!
      Отрисовывает модель в положении x,y,z относительно текущей позиции OpenGL с масштабом scale
    */
    virtual void draw(GLfloat x, GLfloat y, GLfloat z, GLfloat scale);
    //! Перегруженная функция
    /*!
      Отрисовывает модель в текущем положении с масштабом scale
    */
    virtual void draw(GLfloat scale);
    //! Загружает модель в память OpenGL
    /*!
    */
    virtual void compile();
};

#endif // DRAW3DS_H
