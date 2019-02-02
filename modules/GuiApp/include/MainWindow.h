#pragma once

#include "BackupHelper.h"
#include "InformationTab.h"
#include "ProfileTab.h"

#include <QWidget>
#include <QSystemTrayIcon>

class QTabWidget;

//
// Declaration of the class TabWidget.
//

class MainWindow
	: public QWidget
{
	Q_OBJECT

	//
	// Constructions.
	//

public:
	//! Constructor.
	MainWindow(const QString &fileName, QWidget *parent = 0);

	//
	// Private interface.
	//

private:
	//! Ignores the usual closing of the application
	void closeEvent(QCloseEvent* event) override;

	//! Creates actions for menuBar
	void createActions();

	//! Creates menu
	void createMenus();

	//
	// Private slots.
	//

	private slots:
	//! Shows the icon of the application in the tray for a certain time
	void ShowMessageInTray();

	//! Responds to mouse clicks on the icon to disappear/appear widget in the system tray
	void TrayIconActivated(QSystemTrayIcon::ActivationReason reason);

	//! Shows information about program WBackuper
	void aboutBackuper();

	//! Show useful information with instructions how use backup functionality
	void help();

	//
	// Private data members.
	//

private:
	ProfileTab* profiles;
	InformationTab* profInformation;
	QSystemTrayIcon* trayIcon_;
	QTabWidget *mainTab_;
	QMenu*   fileMenu_;
	QMenu* viewMenu_;
	QMenu *helpMenu_;
	QAction *createNewProfile_;
	QAction *addExistedProfile_;
	QAction *refresh_;
	QAction *aboutProgram_;
	QAction *help_;
	BackupHelper::ThreadBackupHelper thrBackupHelper_;
	QLabel *labAboutProfram;
	QLabel *labHelp;
};


