#ifndef GLMUSHROOM_LOGGER_H
#define GLMUSHROOM_LOGGER_H

#include <QString>
#include <QFile>

//! Вспомогательный класс для ведения отчетов во время выполнения программы

/*!
  Использует стандартный аппарат вывода в файл для отображения состояния выполнения программы
*/
class glmushroom_logger
{
public:
	//! Конструктор
	glmushroom_logger(QString logFileName);
	//! Деструктор
	~glmushroom_logger();
	//! Оператор вывода строки отчета в файл
	void operator << (QString logString);
private:
	//! Текущий файл отчета
	QFile logfile;
	//! Индикатор успешного открытия
	bool opened;
};

#endif // GLMUSHROOM_LOGGER_H
