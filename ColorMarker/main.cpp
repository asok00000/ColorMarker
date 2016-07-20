#include "colormarker.h"
#include <QtWidgets/QApplication>
#include <QLibrary>
#include <QDebug>

#include "utility.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QLibrary mylib("./Resources/PrScrn.dll");
	if (mylib.load())
	{
		Utility::printScreen = (PrintScreen)mylib.resolve("PrScrn");
		if (Utility::printScreen)
		{
			qDebug() << "load print screen API success.";
		}
		else
		{
			qDebug() << "load print screen API failed.";
		}
	}
	else
	{
		qDebug() << "load print screen API failed.";
	}
	ColorMarker w;
	w.show();
	return a.exec();
}
