#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	try
	{
		MainWindow tabdialog("fileName");

		tabdialog.show();

		app.exec();
	}
	catch (std::exception e)
	{
		QMessageBox::warning(0, "Warning", e.what());
	}

	return 0;
}
