#pragma once

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

} // namespace BackupManager
