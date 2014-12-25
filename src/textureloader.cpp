#include "textureloader.h"

QImage loadImage(QString filename){
	QImage result(filename);
	result = result.scaled(128,128);
	for (int x = 0; x < result.width(); x++)
		for (int y = 0; y < result.height(); y++)
		{
		QRgb pix =  result.pixel(x,y);
		int r = qRed(pix);
		int g = qGreen(pix);
		int b = qBlue(pix);
		result.setPixel(x,y,qRgb(b,g,r));
	}
	return result;
}
