#include "MyRenderWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyRenderWindow w;
	w.show();
	return a.exec();
}
