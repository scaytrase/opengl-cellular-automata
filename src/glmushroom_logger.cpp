#include "glmushroom_logger.h"
#include <QDateTime>

/*!
  Создает экземляр логгера в файле logFileName. Если не удается - лог не будет вестить
*/
glmushroom_logger::glmushroom_logger(QString logFileName /*!< Имя лог-файла */){
    logfile.setFileName (logFileName);
    opened = logfile.open(QIODevice::Append | QIODevice::Text);
}

/*!
  Закрывает лог файл перед завершением программы.
*/
glmushroom_logger::~glmushroom_logger(){
    if (opened) logfile.close();
}


/*!
  Опрератор вывода в лог-файл
*/
void glmushroom_logger::operator <<(QString logString/*!< Выводимая строка */){
	if (opened) {
		QDateTime now;
		now = now.currentDateTime();
		QString now_format = now.toString ("'['ddd dd\\mm\\yy hh:mm:ss:zzz']:'");
		logfile.write(now_format.toUtf8());
		logfile.write(logString.toUtf8());
		logfile.write("\n");
		logfile.flush();
	}
}
