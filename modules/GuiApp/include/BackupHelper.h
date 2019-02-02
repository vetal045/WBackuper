#pragma once

#include "BackupManager.h"

#include <QWidget>
#include <QDate>
#include <QTime>
#include <QThread>

class QTimer;

namespace BackupHelper
{
	//
	// Declaration of the class DateProfileBackup.
	//

	class DateProfileBackup
	{
		//
		// Constructions.
		//

	public:
		DateProfileBackup();

		//
		// Public methods.
		//

	public:

		void work();
		const QString& getProfileName() const;
		const QDateTime& getDateTimeProfileBackup() const;
		void Clear();

		//
		// Private methods.
		//

	private:

		void DefiningNearestBackupByDateTime();

		//
		// Private data members.
		//

	private:
		QDateTime nearestDateTime;
		QDate lastDateProfBackup;
		QString profileName;
		int minCountDays;
		int minTime;
	};

	//
	// Declaration of the class BackupHelper.
	//

	class BackupHelper : public QObject
	{
		Q_OBJECT

			//
			// Constructions.
			//

	public:
		BackupHelper();

		//
		// Private interface.
		//

	public:
		//! Backups profile by name
		void BackupProfile(const QString& profileName);

		//! Starts work of timer for first profile to backup
		void startTimerToBP();

		//! Updates defining the nearest profile to backup and starts timer
		void backupNBProfiles();

		//! Rewrites profile's date and time for next backup
		void RewriteDateTimeBackupProfile(const QString& profileName);

		//! Starts work
		void run();

		//
		// Private slots.
		//

		private slots:
		//! Restarts work of timer
		void rerunTimerToBP();

		//
		// Private data members.
		//

	private:
		DateProfileBackup profile_;
		QTimer *tmr_;
	};

	//
	// Declaration of the class BackupHelper.
	//

	class ThreadBackupHelper : public QThread
	{
		Q_OBJECT

		//
		// Protected methods.
		//

	protected:
		void run();

		//
		// Private data members.
		//

	private:
		BackupHelper bh_;
	};

}