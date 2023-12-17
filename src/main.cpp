#include "Application.h"
#include <QtWidgets/QApplication>

// 无需多言 :)
int main(int argc,char* argv[]) {
	QApplication app(argc,argv);
	Application mainwindow;
	mainwindow.show();
	return app.exec();
}
