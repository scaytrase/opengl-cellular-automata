#ifndef GLMUSHROOM_MODEL_H
#define GLMUSHROOM_MODEL_H

#include "glmushroom_logger.h"
#include <QVector>

//! Типы событий модели.

/*! Перечисление используемых событий модели. */
enum model_Event{
    EVENT_TOUCH = 0,  /*!< Пустое событие */
    EVENT_TICK,       /*!< Шаг времени */
    EVENT_SUBTICK,    /*!< Подшаг времени */
    EVENT_RELOAD      /*!< Событие перезагрузки модели */
};

static float MUSHROOM_SUBSTEP = 0.01;

//! Класс модели приложения
/*!
Содержит основной аппарат для манипуляции с матрицей
*/

/*
  Содержит основной аппарат для манипуляции с матрицей
*/
class glmushroom_model
{
public:
    //! Конструктор, принимает указатель на класс logger для ведения отчета.
    glmushroom_model(glmushroom_logger *main_logger);
    //! Размер поля по горизонтали.
    int sizeX() const;
    //! Размер поля по вертикали.
    int sizeY() const;
    //! Возвращает истину, если модель успешно проинициализирована.
    bool success_init() const;
    //! Возвращает данные модели.
    QVector< QVector <float> > get_data() const;
    //! Принимает сигнал к действию.
    void send_Event(model_Event event);
    //! Случайная функция.
    int myrand(int count);
private:
    bool init();
    //! Данные модели.
    QVector< QVector <float> > main_field;
    //! Размер поля по горизонтали.
    int field_size_x;
    //! Размер поля по вертикали.
    int field_size_y;
    //! Была ли инициализация успешной.
    bool inited;
    //! Логгер экземпляра.
    glmushroom_logger *logger;
    //! Расчет следующего шага
    QVector< QVector <float> > next_age();
    //! Расчет устаревания гриба
    QVector< QVector <float> > next_subage();
};
#endif // GLMUSHROOM_MODEL_H
