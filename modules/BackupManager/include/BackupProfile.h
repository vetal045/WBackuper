#pragma once

#include <QString>
#include <QStringList>

namespace BackupManager
{

//
// Declaration of the BackupProfile class.
//

class BackupProfile
{
	//
	// Public interface.
	//

public:
	//! Returns empty directory postfix.
	QString GetEmptyDirPostfix() const;
	//! Returns profile's name.
	QString GetProfileName() const;
	//! Returns path to backup file.
	QString GetBackupFilePath() const;
	//! Returns paths to backuped files.
	const QStringList& GetBackupedFilesPath() const;
	//! Returns chunk size.
	quint64 GetChunkSize() const;
	//! Returns data compression level.
	qint32 GetCompressionLevel() const;
	//! Returns option of data backup.
	const QString& GetOptionProfileBackup() const;
	//! Returns date of data backup.
	const QString& GetDateProfileBackup() const;
	//! Returns time of data backup.
	const QString& GetTimeProfileBackup() const;
	//! Returns date and time of the last data backup.
	const QString& GetDateTimeProfileBackup() const;

	//! Sets profile's name.
	void SetProfileName(const QString& name);
	//! Sets path to backup file/.
	void SetBackupFilePath(const QString& path);
	//! Sets paths to backupe files.
	void SetBackupedFilesPath(const QStringList& list);
	//! Adds path to backuped files list.
	void AddBackupedFilePath(const QString& path);
	//! Adds path to backuped folder.
	void AddBackupedFolderPath(const QString& path);
	//! Sets chunk size.
	void SetChunkSize(const quint64 chunkSIze);
	//! Sets data compression level.
	void SetCompressionLevel(const qint32 level);
	//! Sets option of data backup.
	void SetOptionProfileBackup(const QString& optionBackup);
	//! Sets date of data backup.
	void SetDateProfileBackup(const QString& dateBackup);
	//! Sets time of data backup.
	void SetTimeProfileBackup(const QString& timeBackup);
	//! Sets date and time of the last data backup.
	void SetDateTimeProfileBackup(const QString& dateTimeBackup);

	//! Loads profile from file.
	void LoadProfile();
	//! Saves profile to the file.
	void SaveProfile() const;


	//
	// Private data members.
	//
private:
	//! Path postfix to save empty directories.
	const QString emptyDirPostfix_ = "!*NotAFile*!";

	//! Profile's name.
	QString profileName_;
	//! Option of profile backup
	QString optionBackup_;
	//! Date of profile backup
	QString dateBackup_;
	//! Time of profile backup
	QString timeBackup_;
	//! Date and time last profile backup
	QString dateTimeBackup_;
	//! Chunk size in MB.
	quint64 chunkSize_ = 256;
	//! Path to backup file.
	QString backupFilePath_;
	//! Paths to backuped files.
	QStringList backupedFilesPath_;
	//! Data compression level.
	qint32 compressionLevel_ = -1; // default.
};

//
// Inline implementation.
//

inline QString BackupProfile::GetEmptyDirPostfix() const
{
	return emptyDirPostfix_;
}

inline QString BackupProfile::GetProfileName() const
{
	return profileName_;
}

inline QString BackupProfile::GetBackupFilePath() const
{
	return backupFilePath_;
}

inline const QStringList& BackupProfile::GetBackupedFilesPath() const
{
	return backupedFilesPath_;
}

inline quint64 BackupProfile::GetChunkSize() const
{
	return chunkSize_;
}

inline qint32 BackupProfile::GetCompressionLevel() const
{
	return compressionLevel_;
}

inline const QString & BackupProfile::GetOptionProfileBackup() const
{
	return optionBackup_;
}

inline const QString & BackupProfile::GetDateProfileBackup() const
{
	return dateBackup_;
}

inline const QString & BackupProfile::GetTimeProfileBackup() const
{
	return timeBackup_;
}

inline const QString & BackupProfile::GetDateTimeProfileBackup() const
{
	return dateTimeBackup_;
}

inline void BackupProfile::SetProfileName(const QString& name)
{
	profileName_ = name;
}

inline void BackupProfile::SetBackupFilePath(const QString& path)
{
	backupFilePath_ = path;
}

inline void BackupProfile::SetBackupedFilesPath(const QStringList& list)
{
	backupedFilesPath_ = list;
}

inline void BackupProfile::AddBackupedFilePath(const QString& path)
{
	backupedFilesPath_.push_back(path);
}

inline void BackupProfile::SetChunkSize(const quint64 chunkSize)
{
	chunkSize_ = chunkSize;
}

inline void BackupProfile::SetCompressionLevel(const qint32 level)
{
	compressionLevel_ = level;
}

inline void BackupProfile::SetOptionProfileBackup(const QString & optionBackup)
{
	optionBackup_ = optionBackup;
}

inline void BackupProfile::SetDateProfileBackup(const QString & dateBackup)
{
	dateBackup_ = dateBackup;
}

inline void BackupProfile::SetTimeProfileBackup(const QString & timeBackup)
{
	timeBackup_ = timeBackup;
}

inline void BackupProfile::SetDateTimeProfileBackup(const QString & dateTimeBackup)
{
	dateTimeBackup_ = dateTimeBackup;
}

} // namespace BackupManager
