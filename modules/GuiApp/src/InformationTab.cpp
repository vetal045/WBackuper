#include "pch.h"

#include "InformationTab.h"
#include "BackupManager.h"

#include <exception>

InformationTab::InformationTab(const QFileInfo &fileInfo, QWidget *parent)
	: QWidget(parent)
{
	QStringList files;
	files << "Profiles" << "Chunk size" << "Compression level" << "Date and time of profile backup";

	tableListFiles_ = new QTableWidget();
	tableListFiles_->setColumnCount(4);
	tableListFiles_->setColumnWidth(2, 150);
	tableListFiles_->setColumnWidth(3, 300);
	tableListFiles_->setShowGrid(true);
	tableListFiles_->setHorizontalHeaderLabels(files);
	tableListFiles_->setEditTriggers(QAbstractItemView::NoEditTriggers);
	tableListFiles_->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableListFiles_->setSelectionMode(QAbstractItemView::SingleSelection);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(tableListFiles_);

	DisplayProfileInformation();

	setLayout(mainLayout);
}

void InformationTab::DisplayProfileInformation()
{
	QString filesPath = QDir::currentPath() + "/Profiles";

	QDir export_folder(filesPath);
	export_folder.setNameFilters(QStringList() << "*.txt");
	QStringList fileList = export_folder.entryList();

	QString strChunkSize, strComprLvl, strDateTime;

	for (auto i = 0; i < fileList.count(); ++i)
	{
		tableListFiles_->insertRow(i);
		QString nameFile = QFileInfo(fileList[i]).baseName();
		itemFiles_ = new QTableWidgetItem(QString(nameFile));
		tableListFiles_->setItem(i, 0, itemFiles_);
		

		strChunkSize = getChunkSize(nameFile);
		itemFiles_ = new QTableWidgetItem(QString(strChunkSize));
		tableListFiles_->setItem(i, 1, itemFiles_);
		
		strComprLvl = getComprLvl(nameFile);
		itemFiles_ = new QTableWidgetItem(QString(strComprLvl));
		tableListFiles_->setItem(i, 2, itemFiles_);

		strDateTime = getDateTimePB(nameFile);
		itemFiles_ = new QTableWidgetItem(QString(strDateTime));
		tableListFiles_->setItem(i, 3, itemFiles_);
	}
}

QString  InformationTab::getChunkSize(const QString & profileName) const
{
	BackupManager::BackupManager backuper;

	backuper.CreateProfile(profileName);
	backuper.ModifyProfile(profileName).LoadProfile();

	QString chunkSize;

	chunkSize = QString::number(backuper.ModifyProfile(profileName).GetChunkSize());

	return chunkSize;
}

QString InformationTab::getComprLvl(const QString & profileName) const
{
	BackupManager::BackupManager backuper;

	backuper.CreateProfile(profileName);
	backuper.ModifyProfile(profileName).LoadProfile();

	QString comprLvl;

	comprLvl = QString::number(backuper.ModifyProfile(profileName).GetCompressionLevel());

	return comprLvl;
}

QString InformationTab::getDateTimePB(const QString & profileName) const
{
	BackupManager::BackupManager backuper;

	backuper.CreateProfile(profileName);
	backuper.ModifyProfile(profileName).LoadProfile();

	QString strDate, strTime;

	strDate = backuper.ModifyProfile(profileName).GetDateProfileBackup();
	strTime = backuper.ModifyProfile(profileName).GetTimeProfileBackup();

	QDateTime dt = QDateTime::fromString(strTime, Qt::ISODate);

	QString time = dt.time().toString();

	QMessageBox::warning(0, "Warning", time+"1");
	
	QString dateTimePB;

	if (strDate == "Empty")
	{
		return strDate;
	}
	else
	{
		dateTimePB = strDate + " " + time;

		return dateTimePB;
	}	
}

void InformationTab::addProfileToInformationPage(const QString& profileName)
{
	int numbNewProfile = tableListFiles_->rowCount();
	tableListFiles_->insertRow(numbNewProfile);
	itemFiles_ = new QTableWidgetItem(profileName);
	tableListFiles_->setItem(numbNewProfile, 0, itemFiles_);

	QString strChunkSize, strComprLvl, strDateTime;

	strChunkSize = getChunkSize(profileName);
	itemFiles_ = new QTableWidgetItem(QString(strChunkSize));
	tableListFiles_->setItem(numbNewProfile, 1, itemFiles_);

	strComprLvl = getComprLvl(profileName);
	itemFiles_ = new QTableWidgetItem(QString(strComprLvl));
	tableListFiles_->setItem(numbNewProfile, 2, itemFiles_);

	strDateTime = getDateTimePB(profileName);
	itemFiles_ = new QTableWidgetItem(QString(strDateTime));
	tableListFiles_->setItem(numbNewProfile, 3, itemFiles_);
}

void InformationTab::deleteProfile(const QString& profileName, int currentIndex)
{
	QString filesPath = QDir::currentPath() + "/Profiles";
	QString entirePath = filesPath + "/" + profileName + ".txt";

	QFile::remove(entirePath);
	tableListFiles_->removeRow(currentIndex);
}

void InformationTab::editProfile(const QString& profileName)
{
	QAbstractItemModel* const mdl = tableListFiles_->model();
	mdl->removeRows(0, mdl->rowCount());

	DisplayProfileInformation();
}

void InformationTab::update()
{
	QAbstractItemModel* const mdl = tableListFiles_->model();
	mdl->removeRows(0, mdl->rowCount());

	DisplayProfileInformation();
}
