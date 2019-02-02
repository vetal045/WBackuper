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
// Declaration of the class TabWidget.
//

class ProfileTab
	: public QWidget
{
	Q_OBJECT

		//
		// Construction and destruction.
		//

public:
	//! Constructor
	ProfileTab(const QFileInfo &fileInfo, QWidget *parent = 0);
	
	//! Destructor
	//~ProfileTab();

	//
	// Private slots.
	//

	private slots:

	//! Shows QWizardPages with different settings for creating new profile
	void ClickedAddNew();

	//! Creates dialog window
	void ClickedAddExisted();

	//! Backups choosed profile
	void ClickedBackup();

	//! Restores choosed profile
	void ClickedRestore();

	//! Deletes selected profile
	void ClickedDelete();

	//! Opens dialog window to edit selected profile
	void ClickedEdit();

	//! Displays all profiles
	void DisplayProfiles();

	//! Emit signal that profile was edited by user
	void emitProfileEdited(const QString& profileName);

	//! Updates data
	void update();

	//
	// Signals.
	//

signals:
	void profileAdded(const QString& profileName);
	void profileDeleted(const QString& profileName, int currentIndex);
	void profileEdited(const QString& profileName);

	//
	// Private data members.
	//				

private:
	QPushButton *buttonAddNew_;
	QPushButton *buttonAddExisted_;
	QPushButton *buttonBackup_;
	int count_ = 0;
	QPushButton *buttonDel_;
	QPushButton *buttonEdit_;
	QStringList headerTableForListProfile_;
	QTableWidgetItem *itemFiles_;
	QLabel *labListSelectedFiles_;
	QPushButton *buttonRestore_;
	QTimer *tmrBackupProfile_;
	QTableWidget *tableListFiles_;
};
