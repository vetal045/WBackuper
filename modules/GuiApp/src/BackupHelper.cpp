#include "BackupHelper.h"
#include "..\include\BackupHelper.h"

#include "pch.h"

#include <QMessageBox>

namespace BackupHelper
{
	DateProfileBackup::DateProfileBackup()
	{

	}

	void DateProfileBackup::work()
	{
		DefiningNearestBackupByDateTime();
	}

	const QString & DateProfileBackup::getProfileName() const
	{
		return profileName;
	}

	const QDateTime & DateProfileBackup::getDateTimeProfileBackup() const
	{
		return nearestDateTime;
	}

	void DateProfileBackup::Clear()
	{
		profileName.clear();
	}

	void DateProfileBackup::DefiningNearestBackupByDateTime()
	{
		Clear();

		QString filesPath = QDir::currentPath() + "/Profiles";

		QDir export_folder(filesPath);
		export_folder.setNameFilters(QStringList() << "*.txt");
		QStringList fileList = export_folder.entryList();

		BackupManager::BackupManager backuper;

		QDate now(QDate::currentDate());
		QDate date;
		QDateTime dateTime;
		QDateTime timeBackup;
		int count = 365, repeat = 0;

		QString strDate, strTime, strLastDateProfBackup;
		QString baseProfileName;

		for (const auto& i : fileList)
		{
			baseProfileName = QFileInfo(i).baseName();

			if (baseProfileName.isEmpty())
			{
				break;
			}

			backuper.CreateProfile(baseProfileName);

			backuper.ModifyProfile(baseProfileName).LoadProfile();

			strDate = backuper.ModifyProfile(baseProfileName).GetDateProfileBackup();

			strTime = backuper.ModifyProfile(baseProfileName).GetTimeProfileBackup();

			date = QDate::fromString(strDate, "dd/MM/yyyy");
			timeBackup = QDateTime::fromString(strTime, Qt::ISODate);

			dateTime.setDate(date);
			dateTime.setTime(timeBackup.time());

			minCountDays = now.daysTo(date);
			minTime = QTime::currentTime().msecsTo(timeBackup.time()) / 1000;

			//QMessageBox::warning(0, "Warning", "Prof - " + baseProfileName);

			if (minCountDays < count && minCountDays > 0)
			{
				count = minCountDays;
				nearestDateTime = dateTime;
				profileName = backuper.ModifyProfile(baseProfileName).GetProfileName();

				//QMessageBox::warning(0, "Warning", "1");
			}

			if (dateTime < nearestDateTime && dateTime.date() >= QDate::currentDate() && dateTime.time() >= QTime::currentTime())
			{
				nearestDateTime = dateTime;
				profileName = backuper.ModifyProfile(baseProfileName).GetProfileName();

				//QMessageBox::warning(0, "Warning", "2");
			}

			if (minCountDays == 0 && dateTime.time() >= QTime::currentTime())
			{
				if (repeat == 0)
				{
					count = minCountDays;
					nearestDateTime = dateTime;
					profileName = backuper.ModifyProfile(baseProfileName).GetProfileName();

					++repeat;

					//QMessageBox::warning(0, "Warning", "3");
				}
				else
				{
					if (dateTime.time() < nearestDateTime.time())
					{
						count = minCountDays;
						nearestDateTime = dateTime;
						profileName = backuper.ModifyProfile(baseProfileName).GetProfileName();

						//QMessageBox::warning(0, "Warning", "4");
					}
				}
			}
		}

		//QMessageBox::warning(0, "Warning", "prof - " + profileName);
	}

	BackupHelper::BackupHelper()
	{
		tmr_ = new QTimer(this);

		connect(tmr_, SIGNAL(timeout()), this, SLOT(rerunTimerToBP()));
	}

	void BackupHelper::BackupProfile(const QString & profileName)
	{
		BackupManager::BackupManager backuper;

		backuper.CreateProfile(profileName);
		backuper.ModifyProfile(profileName).LoadProfile();
		backuper.BackupData(profileName);

		QDateTime now = QDateTime::currentDateTime();
		QString dateTimeProfBackup = now.toString();

		backuper.ModifyProfile(profileName).SetDateTimeProfileBackup(dateTimeProfBackup);
		backuper.GetProfile(profileName).SaveProfile();
	}

	void BackupHelper::startTimerToBP()
	{
		profile_.work();

		if (profile_.getProfileName() != "")
		{
			QDateTime dateTime_ = profile_.getDateTimeProfileBackup();
			QDateTime now(QDate::currentDate(), QTime::currentTime());

			int numbMillisec = (now.msecsTo(dateTime_));

			//QMessageBox::warning(0, "Warning", now.toString() + " " + dateTime_.toString() + " = " + QString::number(numbMillisec));

			if (numbMillisec <= 0)
			{
				//QMessageBox::warning(0, "Warning", "--------------");
				profile_.work();
			}
			else
			{
				tmr_->setInterval(numbMillisec);
				tmr_->start();
			}
		}
	}

	void BackupHelper::rerunTimerToBP()
	{
		QString profileName = profile_.getProfileName();

		//QMessageBox::warning(0, "Warning", "Start - " + profileName);

		if (!profileName.isEmpty())
		{
			BackupProfile(profileName);
			RewriteDateTimeBackupProfile(profileName);

			//QMessageBox::warning(0, "Warning", "Backuped. \t");
		}

		profile_.work();

		QString newProfileName = profile_.getProfileName();

		if ((!newProfileName.isEmpty()) && (newProfileName != profileName))
		{
			QDateTime dateTime_ = profile_.getDateTimeProfileBackup();
			QDateTime now(QDate::currentDate(), QTime::currentTime());

			int numbMillisec = now.msecsTo(dateTime_);

			tmr_->start(numbMillisec);
			//QMessageBox::warning(0, "Warning", "Timer starts work. \t");
		}
	}

	void BackupHelper::backupNBProfiles()
	{
		QString filesPath = QDir::currentPath() + "/Profiles";

		QDir export_folder(filesPath);
		export_folder.setNameFilters(QStringList() << "*.txt");
		QStringList fileList = export_folder.entryList();

		QString profileName, strDate, strTime, strLastBackup, strOption;

		QDate date;
		QDateTime dateTime, dateTimeLastBackup;
		QDateTime now(QDateTime::currentDateTime());

		BackupManager::BackupManager backuper;

		for (const auto& i : fileList)
		{
			profileName = QFileInfo(i).baseName();

			backuper.CreateProfile(profileName);
			backuper.ModifyProfile(profileName).LoadProfile();

			strDate = backuper.ModifyProfile(profileName).GetDateProfileBackup();
			strTime = backuper.ModifyProfile(profileName).GetTimeProfileBackup();
			strLastBackup = backuper.ModifyProfile(profileName).GetDateTimeProfileBackup();
			strOption = backuper.ModifyProfile(profileName).GetOptionProfileBackup();

			date = QDate::fromString(strDate, "dd/MM/yyyy");
			dateTime = QDateTime::fromString(strTime, Qt::ISODate);

			dateTimeLastBackup = QDateTime::fromString(strLastBackup, Qt::ISODate);

			dateTime.setDate(date);
			dateTime.setTime(dateTime.time());

			const qint64 days = dateTime.daysTo(now);

			//QMessageBox::warning(0, "Warning", "Profile - " + profileName + "; " + strDate + " " + strTime
			//	+ "\n Time waiting - " + days);


			if (strOption == "Every day")
			{
				if (days < 0)
				{
					BackupProfile(profileName);
					RewriteDateTimeBackupProfile(profileName);

					//QMessageBox::warning(0, "Warning", "Backuped(Every day) - " + profileName);
				}
			}

			if (strOption == "Every week")
			{
				if (days < 0)
				{
					BackupProfile(profileName);
					RewriteDateTimeBackupProfile(profileName);

					//QMessageBox::warning(0, "Warning", "Backuped(Every week) - " + profileName);
				}
			}

			if (strOption == "On the specified date")
			{
				if (days < 0 && strLastBackup == "Not backuped")
				{
					BackupProfile(profileName);
					//QMessageBox::warning(0, "Warning", "Backuped - " + profileName);
				}
			}
		}
	}

	void BackupHelper::RewriteDateTimeBackupProfile(const QString & profileName)
	{
		BackupManager::BackupManager backuper;

		backuper.CreateProfile(profileName);
		backuper.ModifyProfile(profileName).LoadProfile();

		QString strOption = backuper.ModifyProfile(profileName).GetOptionProfileBackup();

		QDateTime now(QDateTime::currentDateTime());
		QDateTime dateTimeProfBackup;

		if (strOption == "Every day")
		{
			dateTimeProfBackup = now.addDays(1);
			//QMessageBox::warning(0, "Warning", "Every day");
		}

		if (strOption == "Every week")
		{
			dateTimeProfBackup = now.addDays(7);
			//QMessageBox::warning(0, "Warning", "Every week");
		}

		if (strOption == "On the specified date")
		{
			dateTimeProfBackup = now;
			//QMessageBox::warning(0, "Warning", "On the specified date");
		}

		QString strDate, strTime;

		strDate = dateTimeProfBackup.date().toString(Qt::ISODate);
		strTime = dateTimeProfBackup.time().toString();

		//QMessageBox::warning(0, "Warning", strDate + "   " + strTime);

		backuper.ModifyProfile(profileName).SetDateProfileBackup(strDate);
		backuper.ModifyProfile(profileName).SetTimeProfileBackup(strTime);
		backuper.GetProfile(profileName).SaveProfile();
	}

	void BackupHelper::run()
	{
		backupNBProfiles();

		startTimerToBP();
	}

	void ThreadBackupHelper::run()
	{
		bh_.run();
	}
}