#pragma once

#include <QWidget>

class QFileInfo;
class QLabel;
class QPushButton;
class QStringList;
class QTimer;
class QTableWidget;
class QTableWidgetItem;

//
// Declaration of the class InformationTab.
//

class InformationTab
	: public QWidget
{
	Q_OBJECT

		//
		// Construction and destruction.
		//

public:
	//! Constructor
	InformationTab(const QFileInfo &fileInfo, QWidget *parent = 0);

	//! Destructor
	//~ProfileTab();

	//
	// Private methods.
	//

private:

	//! Displays information about all profiles
	void DisplayProfileInformation();

	//! Gets chunk size from file of the specified profile
	QString getChunkSize(const QString& profileName) const;

	//! Gets compression level from file of the specified profile
	QString getComprLvl(const QString& profileName) const;

	//! Gets date and time backup from file by name of the specified profile
	QString getDateTimePB(const QString& profileName) const;


	//
	// Private slots.
	//

	private slots:

	//! Adds details of created or added new profile on Information Tab 
	void addProfileToInformationPage(const QString& profileName);

	//! Deletes profile
	void deleteProfile(const QString& profileName, int currentIndex);

	//! Edits profile
	void editProfile(const QString& profileName);

	//! Updates data
	void update();

	//
	// Private data members.
	//				

private:
	int count_ = 0;
	QStringList headerTableForListProfile_;
	QTableWidgetItem *itemFiles_;
	QTableWidget *tableListFiles_;
};
