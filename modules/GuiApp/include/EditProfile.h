#pragma once

#include <QWidget>
#include <QDialog>

class QLabel;
class QLineEdit;
class QSlider;
class QSpinBox;
class QDateTimeEdit;
class QDialogButtonBox;

//
// Declaration of the class EditProfile.
//

class EditProfile : public QDialog
{
	Q_OBJECT

	//
	// Constructions and destruction.
	//

public:
	EditProfile(QString profileName, QWidget *parent = 0);
	~EditProfile();

	//
	// Private methods.
	//

private:
	//! Gets settings of the profile
	void readProfileData(const QString& profileName);
	//! Sets values of settings to elements of edit page
	void establishProfileValues();
	//! Saves changes to profile
	void saveChangedSettingsBP();
	//! Returns true if use smth changes
	bool isModified();
	//! Gets profile name
	const QString& getProfileName() const;
	//! Gets profile chunk size
	const quint64 getChunkSize() const;
	//! Gets profile compression level
	const quint32 getComprLvl() const;
	//! Gets date and time profile's backup
	const QDateTime& getDateTime() const;

private slots:
	//! Saves changes to profile 
	void clickedApply();
	//! Closes edit page
	void reject() override;
	//! Saves changes to profile and close window
	void accept() override;
	//! Event of closing window
	void closeEvent(QCloseEvent *event) override;

signals:
	void profileEdited(const QString& profileName);

	//
	// Private data members.
	//

private:
	QPushButton *buttonApply;
	QLabel *labProfileName_;
	QLabel *labChunkSize_;
	QLabel *labLvlCompr_;
	QLabel *labViewComprLvl_;
	QLabel *labDateTimeBackup_;
	QLineEdit *lineProfileName_;
	QSpinBox *sbChunkSize_;
	QSlider *sliderComprLvl_;
	QDateTimeEdit *lineDateTimeBackup_;
	QString profileName_;
	quint64 chunkSize_;
	quint32 comprLvl_;
	QDateTime dateTimeBackup_;
	QDialogButtonBox *buttonBox;
};


