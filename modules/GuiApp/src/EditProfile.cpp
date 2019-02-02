#include "EditProfile.h"
#include "BackupManager.h"

#include <QDialogButtonBox>

EditProfile::EditProfile(QString profileName, QWidget *parent) : QDialog(parent), profileName_(profileName)
{
	labProfileName_ = new QLabel(tr("Name"));
	labChunkSize_ = new QLabel(tr("Chunk size"));
	labLvlCompr_ = new QLabel(tr("Compression level"));
	labDateTimeBackup_ = new QLabel(tr("Date and time"));
	labViewComprLvl_ = new QLabel();

	lineProfileName_ = new QLineEdit();

	sbChunkSize_ = new QSpinBox();
	sbChunkSize_->setMaximum(10000);
	sbChunkSize_->setRange(0, 65536);

	sliderComprLvl_ = new QSlider();
	sliderComprLvl_ = new QSlider(Qt::Horizontal);
	sliderComprLvl_->setRange(-1, 10);
	sliderComprLvl_->setPageStep(1);
	sliderComprLvl_->setValue(-1);
	sliderComprLvl_->setTickInterval(1);
	sliderComprLvl_->setTickPosition(QSlider::TicksBelow);

	lineDateTimeBackup_ = new QDateTimeEdit();

	buttonBox = new QDialogButtonBox(Qt::Horizontal);

	buttonApply = new QPushButton(tr("Apply"));

	buttonBox->addButton(tr("OK"), QDialogButtonBox::AcceptRole);
	buttonBox->addButton(buttonApply, QDialogButtonBox::ApplyRole);
	buttonBox->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);

	connect(sliderComprLvl_, SIGNAL(valueChanged(int)), labViewComprLvl_, SLOT(setNum(int)));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonApply, SIGNAL(clicked()), this, SLOT(clickedApply()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *layoutProfileName = new QHBoxLayout;
	QHBoxLayout *layoutComprLvl = new QHBoxLayout;
	QVBoxLayout *layoutChunkSize = new QVBoxLayout;
	QHBoxLayout *layoutDateTime = new QHBoxLayout;

	layoutProfileName->addWidget(labProfileName_);
	layoutProfileName->addWidget(lineProfileName_);

	layoutComprLvl->addWidget(labLvlCompr_);
	layoutComprLvl->addWidget(sliderComprLvl_);
	layoutComprLvl->addWidget(labViewComprLvl_);

	layoutChunkSize->addWidget(labChunkSize_);
	layoutChunkSize->addWidget(sbChunkSize_);

	layoutDateTime->addWidget(labDateTimeBackup_);
	layoutDateTime->addWidget(lineDateTimeBackup_);

	QGridLayout *butLayout = new QGridLayout;
	butLayout->addWidget(buttonBox, 4, 0, 1, 3);
	
	layout->addLayout(layoutProfileName);
	layout->addSpacing(10);
	layout->addLayout(layoutChunkSize);
	layout->addSpacing(10);
	layout->addLayout(layoutComprLvl);
	layout->addSpacing(10);
	layout->addLayout(layoutDateTime);
	layout->addSpacing(50);
	mainLayout->addLayout(layout);
	mainLayout->addLayout(butLayout);

	readProfileData(profileName_);
	establishProfileValues();

	setLayout(mainLayout);

	setWindowTitle(tr("Edit"));
}

EditProfile::~EditProfile()
{
	delete labProfileName_;
	delete labChunkSize_;
	delete labLvlCompr_;
	delete labDateTimeBackup_;
	delete lineDateTimeBackup_;
	delete sliderComprLvl_;
	delete sbChunkSize_;
	delete buttonBox;
}

void EditProfile::readProfileData(const QString & profileName)
{
	BackupManager::BackupManager backuper;

	backuper.CreateProfile(profileName);
	backuper.ModifyProfile(profileName).LoadProfile();

	profileName_ = profileName;

	QString chunkSize, comprLvl;

	chunkSize_ = backuper.ModifyProfile(profileName).GetChunkSize();
	comprLvl_ = backuper.ModifyProfile(profileName).GetCompressionLevel();

	QString strDate, strTime;

	strDate = backuper.ModifyProfile(profileName).GetDateProfileBackup();
	strTime = backuper.ModifyProfile(profileName).GetTimeProfileBackup();

	dateTimeBackup_ = QDateTime::fromString(strTime, Qt::ISODate);
	dateTimeBackup_.setDate(QDate::fromString(strDate, "dd/MM/yyyy"));
}

void EditProfile::establishProfileValues()
{
	lineProfileName_->setText(profileName_);
	sbChunkSize_->setValue(chunkSize_);
	sliderComprLvl_->setValue(comprLvl_);
	lineDateTimeBackup_->setDate(dateTimeBackup_.date());
	lineDateTimeBackup_->setTime(dateTimeBackup_.time());
}

void EditProfile::saveChangedSettingsBP()
{
	if (this->isModified())
	{
		BackupManager::BackupManager backuper;

		backuper.CreateProfile(profileName_);
		backuper.ModifyProfile(profileName_).LoadProfile();

		QString newProfileName = lineProfileName_->text();
		chunkSize_ = sbChunkSize_->value();
		comprLvl_ = sliderComprLvl_->value();
		dateTimeBackup_.setDate(lineDateTimeBackup_->date());
		dateTimeBackup_.setTime(lineDateTimeBackup_->time());

		//backuper.ModifyProfile(profileName_).SetProfileName(newProfileName);

		QString  strDate, strTime;

		strDate = getDateTime().date().toString();
		strTime = getDateTime().time().toString();

		QMessageBox::warning(0, "Warning", "Profile - " + profileName_ + "  date and time - "+strDate+" "+strTime);

		backuper.ModifyProfile(profileName_).SetChunkSize(getChunkSize());
		backuper.ModifyProfile(profileName_).SetCompressionLevel(getComprLvl());
		backuper.ModifyProfile(profileName_).SetDateProfileBackup(strDate);
		backuper.ModifyProfile(profileName_).SetTimeProfileBackup(strTime);

		backuper.GetProfile(profileName_).SaveProfile();

		emit profileEdited(profileName_);
	}
}

bool EditProfile::isModified()
{
	quint64 chunkSize = sbChunkSize_->value();
	quint32 comprLvl = sliderComprLvl_->value();
	QDate date = lineDateTimeBackup_->date();
	QTime time  = lineDateTimeBackup_->time();

	if (lineDateTimeBackup_->text() != getProfileName() || chunkSize == getChunkSize() 
		|| comprLvl == getComprLvl() || date != getDateTime().date() || time != getDateTime().time())
	{
		return true;
	}
	else
	{
		return false;
	}
}

const QString & EditProfile::getProfileName() const
{
	return profileName_;
}

const quint64 EditProfile::getChunkSize() const
{
	return chunkSize_;
}

const quint32 EditProfile::getComprLvl() const
{
	return comprLvl_;
}

const QDateTime & EditProfile::getDateTime() const
{
	return dateTimeBackup_;
}

void EditProfile::reject()
{
	this->hide();
}

void EditProfile::accept()
{
	saveChangedSettingsBP();

	this->hide();
}

void EditProfile::closeEvent(QCloseEvent * event)
{
	if (this->isModified()) 
	{
		int r = QMessageBox::warning(this,
			tr("Save"),
			tr("Do you want to save the changes of profile "
				"before closing?"),
			QMessageBox::Save
			| QMessageBox::Cancel);

		if (r == QMessageBox::Save) 
		{
			this->accept();
		}
		else 
		{
			event->ignore();
		}
	}
}

void EditProfile::clickedApply()
{
	saveChangedSettingsBP();
}