#include "pch.h"

#include "BackupManager.h"
#include "ProfileTab.h"
#include "QWizardNewProfile.h"
#include "ProfilesFolder.h"
#include "EditProfile.h"

#include <exception>

ProfileTab::ProfileTab(const QFileInfo &fileInfo, QWidget *parent)
	: QWidget(parent)
{
	buttonAddNew_ = new QPushButton("Add &new");
	buttonAddExisted_ = new QPushButton("Add &existed");
	buttonDel_ = new QPushButton("&Delete");
	buttonRestore_ = new QPushButton("&Restore");
	buttonEdit_ = new QPushButton("&Edit");
	buttonBackup_ = new QPushButton("&Backup");

	QVBoxLayout *buttonLayout = new QVBoxLayout;
	buttonLayout->addWidget(buttonAddNew_);
	buttonLayout->addWidget(buttonAddExisted_);
	buttonLayout->addStretch(1);
	buttonLayout->addWidget(buttonBackup_);
	buttonLayout->addWidget(buttonRestore_);
	buttonLayout->addWidget(buttonEdit_);
	buttonLayout->addWidget(buttonDel_);

	QStringList files;
	files << "Profiles";

	tableListFiles_ = new QTableWidget();
	tableListFiles_->setColumnCount(1);
	tableListFiles_->setShowGrid(true);
	tableListFiles_->setSelectionMode(QAbstractItemView::SingleSelection);
	tableListFiles_->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableListFiles_->setHorizontalHeaderLabels(files);
	tableListFiles_->horizontalHeader()->setStretchLastSection(true);
	tableListFiles_->setEditTriggers(QAbstractItemView::NoEditTriggers);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(tableListFiles_);
	mainLayout->addLayout(buttonLayout);

	tableListFiles_->setFixedSize(650, 500);
	buttonAddNew_->setFixedSize(150, 80);
	buttonAddExisted_->setFixedSize(150, 80);
	buttonBackup_->setFixedSize(130, 70);
	buttonRestore_->setFixedSize(130, 70);
	buttonEdit_->setFixedSize(130, 70);
	buttonDel_->setFixedSize(130, 70);

	connect(buttonAddNew_, SIGNAL(clicked()), this, SLOT(ClickedAddNew()));
	connect(buttonAddExisted_, SIGNAL(clicked()), SLOT(ClickedAddExisted()));
	connect(buttonBackup_, SIGNAL(clicked()), this, SLOT(ClickedBackup()));
	connect(buttonRestore_, SIGNAL(clicked()), this, SLOT(ClickedRestore()));
	connect(buttonDel_, SIGNAL(clicked()), SLOT(ClickedDelete()));
	connect(buttonEdit_, SIGNAL(clicked()), SLOT(ClickedEdit()));

	DisplayProfiles();

	setLayout(mainLayout);
}
/*
ProfileTab::~ProfileTab()
{
	delete buttonAddNew_;
	delete buttonAddExisted_;
	delete buttonBackup_;
	delete buttonDel_;
	delete buttonEdit_;
	delete itemFiles_;
	delete labListSelectedFiles_;
	delete buttonRestore_;
	delete tmrBackupProfile_;
	delete tableListFiles_;
}
*/
void ProfileTab::ClickedAddNew()
{
	CheckProfilesFolder();

	QScopedPointer<WizardNewProfile> newProf(new WizardNewProfile);
	newProf->show();
	buttonAddNew_->setDisabled(true);

	if ((newProf->exec() == 0) || (newProf->isActiveWindow() == 0))
	{
		if (newProf.data()->field("profileName").toString() != "")
		{
			int numbNewProfile = tableListFiles_->rowCount();
			tableListFiles_->insertRow(numbNewProfile);
			itemFiles_ = new QTableWidgetItem(QString(newProf.data()->field("profileName").toString()));
			tableListFiles_->setItem(numbNewProfile, 0, itemFiles_);

			emit profileAdded(newProf.data()->field("profileName").toString());
		}

		buttonAddNew_->setEnabled(true);
	}
}

void ProfileTab::ClickedAddExisted()
{
	QString filePath = QFileDialog::getOpenFileName(
		this,
		tr("Select backup profile"),
		"C://",
		"All files (*.txt*)"
	);

	if (filePath != "")
	{
		QFile fileProfile(filePath);

		if (!fileProfile.open(QIODevice::ReadOnly))
		{
			qDebug() << "Open error file to reading." << endl;;
			QMessageBox::critical(0, "Error", "Unknown error. \t");
		}
		else
		{
			QByteArray block = fileProfile.read(23);
			QString firstStringFile = "# Config section start.";

			if (firstStringFile != block)
			{
				QMessageBox::critical(0, "Error", "You have selected an unsuitable file. \t");

				return;
			}
			else
			{
				QString fileName = QFileInfo(filePath).baseName();
				QString newFilePath = QDir::currentPath() + "/Profiles/" + fileName + ".txt";

				QFile::copy(filePath, newFilePath);

				int numExistedProfile = tableListFiles_->rowCount();
				tableListFiles_->insertRow(numExistedProfile);
				itemFiles_ = new QTableWidgetItem(QString(fileName));
				tableListFiles_->setItem(numExistedProfile, 0, itemFiles_);

				emit profileAdded(fileName);
			}
		}
	}
}

void ProfileTab::ClickedBackup()
{
	if (tableListFiles_->currentIndex().isValid() != true)
	{
		QMessageBox msgBox;
		msgBox.setText("Noone profile chose.");
		msgBox.exec();
	}
	else
	{
		BackupManager::BackupManager backuper;

		QModelIndex currentIndex = tableListFiles_->currentIndex();

		QString fileName = tableListFiles_->item(currentIndex.row(), 0)->text();

		backuper.CreateProfile(fileName);
		backuper.ModifyProfile(fileName).LoadProfile();
		backuper.BackupData(fileName);

		QDateTime now = QDateTime::currentDateTime();
		QString dateTimeProfBackup = now.toString();

		backuper.ModifyProfile(fileName).SetDateTimeProfileBackup(dateTimeProfBackup);
		backuper.GetProfile(fileName).SaveProfile();
	}
}

void ProfileTab::ClickedRestore()
{
	if (tableListFiles_->currentIndex().isValid() != true)
	{
		QMessageBox msgBox;
		msgBox.setText("Noone profile chose.");
		msgBox.exec();
	}
	else
	{
		BackupManager::BackupManager backuper;

		QModelIndex currentIndex = tableListFiles_->currentIndex();

		QString fileName = tableListFiles_->item(currentIndex.row(), 0)->text();

		backuper.CreateProfile(fileName);

		backuper.ModifyProfile(fileName).LoadProfile();

		backuper.RestoreData(fileName);
	}
}


void ProfileTab::ClickedDelete()
{
	if (tableListFiles_->currentIndex().isValid() == true)
	{
		QMessageBox::StandardButton resBtn = QMessageBox::warning(this, "Warning about deleting profile",
			tr("Do you really want to delete this profile?"),
			QMessageBox::Cancel | QMessageBox::Yes,
			QMessageBox::Yes);
		 
		if (resBtn == QMessageBox::Yes)
		{
			int currentRow = tableListFiles_->currentRow();
			QModelIndex currentIndex = tableListFiles_->currentIndex();

			QString fileName = tableListFiles_->item(currentIndex.row(), 0)->text();
			QString filesPath = QDir::currentPath() + "/Profiles";
			QString entirePath = filesPath + "/" + fileName + ".txt";

			QFile::remove(entirePath);
			tableListFiles_->removeRow(currentRow);

			emit profileDeleted(fileName, currentRow);
		}
		else
			QDialog::IgnoreMask;
	}

	return;
}

void ProfileTab::ClickedEdit()
{
	if (tableListFiles_->currentIndex().isValid() != true)
	{
		QMessageBox msgBox;
		msgBox.setText("Noone profile chose.");
		msgBox.exec();
	}
	else
	{
		QModelIndex currentIndex = tableListFiles_->currentIndex();
		QString fileName = tableListFiles_->item(currentIndex.row(), 0)->text();

		EditProfile *editWindow = new EditProfile(fileName);
		editWindow->show();

		connect(editWindow, SIGNAL(profileEdited(const QString&)), this, SLOT(emitProfileEdited(const QString&)));
	}
}

void ProfileTab::DisplayProfiles()
{	
	QString filesPath = QDir::currentPath()+"/Profiles";
	
	QDir export_folder(filesPath);
	export_folder.setNameFilters(QStringList() << "*.txt");
	QStringList fileList = export_folder.entryList();

	for (auto i = 0; i < fileList.count(); ++i)
	{
		tableListFiles_->insertRow(i);
		QString nameFile = QFileInfo(fileList[i]).baseName();
		itemFiles_ = new QTableWidgetItem(QString(nameFile));
		tableListFiles_->setItem(i, 0, itemFiles_);
	}
}

void ProfileTab::emitProfileEdited(const QString& profileName)
{
	emit profileEdited(profileName);
}

void ProfileTab::update()
{
	QAbstractItemModel* const mdl = tableListFiles_->model();
	mdl->removeRows(0, mdl->rowCount());

	DisplayProfiles();
}



