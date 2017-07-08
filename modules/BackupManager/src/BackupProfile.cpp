#include "pch.h"

#include "BackupProfile.h"
#include "ConfigManager.h"

namespace BackupManager
{

void BackupProfile::LoadProfile()
{
	ConfigManager config;
	config.Load(QString("%1.txt").arg(profileName_), true);

	backupFilePath_ = config.GetValue("Result");
	chunkSize_ = config.GetValue("ChunkSize", "256").toUInt();
	compressionLevel_ = config.GetValue("Compression", "0").toInt();
	backupedFilesPath_ = config.GetValue("Files", "").split(';');
}

void BackupProfile::SaveProfile() const
{
	ConfigManager config;

	config.SetValue("Result", backupFilePath_);
	config.SetValue("ChunkSize", QString::number(chunkSize_));
	config.SetValue("Compression", QString::number(compressionLevel_));
	config.SetValue("Files", backupedFilesPath_.join(';'));

	config.Save(QString("%1.txt").arg(profileName_));
}

void BackupProfile::AddBackupedFolderPath(const QString& path)
{
	QDir dir(path);

	if (!dir.exists())
	{
		return;
	}

	auto entries = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

	auto hasEntries = entries.length() > 0;

	for (const auto& entry : entries)
	{
		AddBackupedFolderPath(path + "/" + entry);
	}

	entries = dir.entryList(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

	hasEntries = hasEntries || entries.length() > 0;

	for (const auto& entry : entries)
	{
		AddBackupedFilePath(path + "/" + entry);
	}

	if (!hasEntries)
	{
		AddBackupedFilePath(path + "/" + emptyDirPostfix_);
	}
}

} // namespace BackupManager
