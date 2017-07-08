#pragma once

#include "BackupProfile.h"

namespace BackupManager
{

//
// Declaration of the BackupManager class.
//

class BackupManager
{
	//
	// Public interface.
	//
public:
	//! Creates a profile with given name.
	void CreateProfile(const QString& profileName);
	//! Returns the profile with name.
	const BackupProfile& GetProfile(const QString& profileName) const;
	//! Returns profile with name to modify.
	BackupProfile& ModifyProfile(const QString& profileName);
	//! Returns true if the profile exists.
	bool Contains(const QString& profileName);

	const QList<BackupProfile>& GetProfiles() const;

	//! Backup all profile's files.
	void BackupData(const QString& profileName);
	//! Restores all profile's files.
	void RestoreData(const QString& profileName);

	//
	// Private methods.
	//
private:
	//! Calculates file size.
	quint64 CalculateFileSize(const QString& file);
	//! Calculates files total size.
	quint64 CalculateFilesSize(const QStringList& files);

	//
	// Private data members.
	//
private:
	//! 1 MB in bytes
	static const quint64 BytesIn1Mb = 1 * 1024 * 1024;
	//! SHA256 required size.
	static const quint64 BytesInSha256 = 256 / 8;
	//! MD5 required size.
	static const quint64 BytesInMd5 = 128 / 8;

	//! Backup file prefix.
	const QString backupPrefix = "Backuper_raw_data";
	//! Backup data prefix.
	const QString backupDataPrefix = "Backuper_data";

	//! List of backup profiles.
	QList<BackupProfile> profiles_;
};

} // namespace BackupManager