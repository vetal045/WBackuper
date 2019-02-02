#include "pch.h"

#include "MainWindow.h"
#include "ProfilesFolder.h"

MainWindow::MainWindow(const QString &fileName, QWidget *parent)
	: QWidget(parent)
{
	CheckProfilesFolder();
	
	QFileInfo fileInfo(fileName);
	profiles = new ProfileTab(fileInfo);
	profInformation = new InformationTab(fileInfo);

	mainTab_ = new QTabWidget();
	mainTab_->addTab(profiles, tr("Profiles"));
	mainTab_->addTab(profInformation, tr("Infrormation"));
	
	createActions();
	createMenus();

	QMenuBar *menuBar_ = new QMenuBar(this);

	menuBar_->addMenu(fileMenu_);
	menuBar_->addMenu(viewMenu_);
	menuBar_->addMenu(helpMenu_);

	menuBar_->show();
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(menuBar_);
	mainLayout->addWidget(mainTab_);
	setLayout(mainLayout);
	
	trayIcon_ = new QSystemTrayIcon(this);

	connect(
		trayIcon_,
		SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		SLOT(TrayIconActivated(QSystemTrayIcon::ActivationReason))
	);

	trayIcon_->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
	trayIcon_->setToolTip(tr("WBackuper"));

	QMenu * menu = new QMenu(this);
	QAction * viewWindow = new QAction(tr("Maximize window"), this);
	QAction * quitAction = new QAction(tr("Exit"), this);

	connect(viewWindow, SIGNAL(triggered()), this, SLOT(show()));
	connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
	connect(profiles, SIGNAL(profileAdded(const QString&)), profInformation, SLOT(addProfileToInformationPage(const QString&)));
	connect(profiles, SIGNAL(profileDeleted(const QString&, int)), profInformation, SLOT(deleteProfile(const QString&, int)));
	connect(profiles, SIGNAL(profileEdited(const QString&)), profInformation, SLOT(editProfile(const QString&)));

	menu->addAction(viewWindow);
	menu->addAction(quitAction);

	trayIcon_->setContextMenu(menu);
	trayIcon_->show();

	QPalette pal(Qt::cyan);
	setPalette(pal);

	setWindowIcon(QIcon("C:/Projects/Backuper/Photos/2.png"));
		
	setWindowTitle(tr("WBackuper"));
	setMinimumHeight(600);
	setMaximumHeight(600);
	setMinimumWidth(900);
	setMaximumWidth(900);

	thrBackupHelper_.start();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (this->isVisible()) 
	{
		event->ignore();
		this->hide();
		QSystemTrayIcon::MessageIcon icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);

		trayIcon_->showMessage("WBackuper",
			"The application is minimized to tray. In order to "
				"expand the application window, click on the application icon in the tray",
			icon,
			2000);
	}
}

void MainWindow::createActions()
{
	aboutProgram_ = new QAction(tr("&About WBackuper"));
	help_ = new QAction(tr("Help"));
	createNewProfile_ = new QAction(tr("New profile"));
	addExistedProfile_ = new QAction(tr("Add profile"));
	refresh_ = new QAction(tr("Refresh"));

	connect(aboutProgram_, SIGNAL(triggered()), this, SLOT(aboutBackuper()));
	connect(help_, SIGNAL(triggered()), this, SLOT(help()));
	connect(createNewProfile_, SIGNAL(triggered()), profiles, SLOT(ClickedAddNew()));
	connect(addExistedProfile_, SIGNAL(triggered()), profiles, SLOT(ClickedAddExisted()));
	connect(refresh_, SIGNAL(triggered()), profiles, SLOT(update()));
	connect(refresh_, SIGNAL(triggered()), profInformation, SLOT(update()));
}

void MainWindow::createMenus()
{
	fileMenu_ = new QMenu(tr("Menu"));
	fileMenu_->addAction(createNewProfile_);
	fileMenu_->addSeparator();
	fileMenu_->addAction(addExistedProfile_);
	fileMenu_->addSeparator();
	fileMenu_->addAction("&Exit", this, SLOT(close()));

	viewMenu_ = new QMenu(tr("View"));
	viewMenu_->addAction(refresh_);

	helpMenu_ = new QMenu(tr("About"));
	helpMenu_->addAction(aboutProgram_);
	helpMenu_->addSeparator();
	helpMenu_->addAction(help_);
}

void MainWindow::TrayIconActivated(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		setVisible(!isVisible());
		break;

	default:
		break;
	}
}

void MainWindow::ShowMessageInTray() {
	trayIcon_->showMessage("Message title", "Message text", QSystemTrayIcon::Information);
}

void MainWindow::aboutBackuper() 
{
	labAboutProfram = new QLabel(tr("WBackuper (formerly known as double V backup-helper\n)" 
		"is a simple tool for backing up files, partitions, hard drives or your system.\n" 
		"The program's straightforward interface will be immediately familiar \n"
		"if you've ever used another backup tool."));

	labAboutProfram->setStyleSheet(QString("font-size: %1px").arg(20));
	labAboutProfram->show();
}

void MainWindow::help() 
{
	labHelp = new QLabel(tr("To see details about WBackuper, go to link: https://github.com/StiventoUser/Backuper"));
	labHelp->setStyleSheet(QString("font-size: %1px").arg(20));
	labHelp->show();
}

