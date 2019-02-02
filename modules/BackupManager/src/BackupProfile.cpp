#include "pch.h"

#include "BackupProfile.h"
#include "ConfigManager.h"

namespace BackupManager
{

void BackupProfile::LoadProfile()
{
	ConfigManager config;
	config.Load(QString("C:/Projects/Backuper/cmake-build/vc14-x64/modules/GuiApp/Profiles/%1.txt").arg(profileName_), true);

	backupFilePath_ = config.GetValue("Result");
	optionBackup_ = config.GetValue("Option");
	dateBackup_ = config.GetValue("Date");
	timeBackup_ = config.GetValue("Time");
	dateTimeBackup_ = config.GetValue("DateTimeLastBackup");
	chunkSize_ = config.GetValue("ChunkSize", "256").toUInt();
	compressionLevel_ = config.GetValue("Compression", "0").toInt();
	backupedFilesPath_ = config.GetValue("Files", "").split(';');
}

void BackupProfile::SaveProfile() const
{
	ConfigManager config;

	config.SetValue("Result", backupFilePath_);
	config.SetValue("Option", optionBackup_);
	config.SetValue("Date", dateBackup_);
	config.SetValue("Time", timeBackup_);
	config.SetValue("DateTimeLastBackup", dateTimeBackup_);
	config.SetValue("ChunkSize", QString::number(chunkSize_));
	config.SetValue("Compression", QString::number(compressionLevel_));
	config.SetValue("Files", backupedFilesPath_.join(';'));

	config.Save(QString("C:/Projects/Backuper/cmake-build/vc14-x64/modules/GuiApp/Profiles/%1.txt").arg(profileName_));
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
