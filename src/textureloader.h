#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <QImage>
//! Функция обертка для загрузки текстур с помощью класса QImage

/*!
  В связи с тем, что в bmp цвета хранятся в BGR, то в случае
использования QImage надо принудительно менять каналы местами
*/
QImage loadImage(QString filename/*!< Имя файла текстуры*/);

#endif // TEXTURELOADER_H
