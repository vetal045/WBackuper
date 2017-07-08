#include "pch.h"

#include "BackupManager.h"
#include "StreamDevice.h"

#include <fstream>
#include <vector>

namespace BackupManager
{

QTextStream qout(stdout);

template<typename T>
void print(const char* str, const T& t)
{
	qout << str << t << "\n";
	qout.flush();
}

void print(const char* str)
{
	qout << str << "\n";
	qout.flush();
}

void print(const QString& str)
{
	qout << str << "\n";
	qout.flush();
}

void BackupManager::CreateProfile(const QString& profileName)
{
	BackupProfile profile;
	profile.SetProfileName(profileName);

	profiles_.push_back(profile);
}

const BackupProfile& BackupManager::GetProfile(const QString& profileName) const
{
	for (const auto& p : profiles_)
	{
		if (p.GetProfileName() == profileName)
		{
			return p;
		}
	}

	throw std::runtime_error(QString("The profile '%1' doesn't exist")
		.arg(profileName).toStdString());
}

BackupProfile& BackupManager::ModifyProfile(const QString& profileName)
{
	for (auto& p : profiles_)
	{
		if (p.GetProfileName() == profileName)
		{
			return p;
		}
	}

	throw std::runtime_error(QString("The profile '%1' doesn't exist")
		.arg(profileName).toStdString());
}

bool BackupManager::Contains(const QString& profileName)
{
	for (auto& p : profiles_)
	{
		if (p.GetProfileName() == profileName)
		{
			return true;
		}
	}

	return false;
}

const QList<BackupProfile>& BackupManager::GetProfiles() const
{
	return profiles_;
}

void BackupManager::BackupData(const QString& profileName)
{
	const auto& profile = GetProfile(profileName);

	const auto& files = profile.GetBackupedFilesPath();
	const auto emptyDirPostfix = profile.GetEmptyDirPostfix();

	auto totalSize = CalculateFilesSize(files);

	auto chunkSize = profile.GetChunkSize() * BytesIn1Mb;
	auto chunkCount = totalSize / chunkSize;
	if ((totalSize % chunkSize) != 0)
		chunkCount += 1;

	StreamFile resFile(profile.GetBackupFilePath(), QIODevice::WriteOnly);

	QCryptographicHash hashAlg(QCryptographicHash::Md5);
	QCryptographicHash mainHashAlg(QCryptographicHash::Md5);

	resFile << backupPrefix;

	print("Writing header");
	// writes file header.
	{
		StreamBuffer headerData(QIODevice::WriteOnly);

		headerData << profile.GetProfileName() << profile.GetCompressionLevel()
			<< profile.GetBackupFilePath() << chunkSize << chunkCount;

		hashAlg.addData(headerData.GetData());
		
		resFile << hashAlg.result() << headerData.GetData();

		mainHashAlg.addData(hashAlg.result());

		hashAlg.reset();
	}
	
	// Reserves an area to main algorithm.
	resFile.Reserve("MainHash", BytesInMd5);

	print("Writing data header");
	// writes data header.
	{
		StreamBuffer buffer(QIODevice::WriteOnly);
		quint64 currentOffset = 0;

		buffer << (quint64)files.length();

		for (const auto& file : files)
		{
			buffer << file << currentOffset;

			if (!file.contains(emptyDirPostfix))
			{
				auto size = CalculateFileSize(file);
				buffer << size;

				currentOffset += size;
			}
			else
			{
				buffer << static_cast<quint64>(0);
			}
		}

		hashAlg.addData(buffer.GetData());

		mainHashAlg.addData(hashAlg.result());

		resFile << hashAlg.result() << buffer.GetData();

		hashAlg.reset();
	}

	resFile << backupDataPrefix;

	print("Writing data");

	quint64 currentChunk = 0;

	StreamBuffer chunkBuff(QIODevice::WriteOnly);
	quint64 shouldRead = chunkSize;

	auto saveChunk = [&]()
	{
		print("Saving chunk: ", currentChunk);

		hashAlg.addData(chunkBuff.GetData());

		mainHashAlg.addData(hashAlg.result());

		resFile << hashAlg.result();
		resFile.WriteRawData(chunkBuff.GetData());

		hashAlg.reset();
		chunkBuff.Clear();

		shouldRead = chunkSize;

		++currentChunk;

		chunkBuff << currentChunk;
	};

	chunkBuff << currentChunk;

	for (const auto& filePath : files)
	{
		if (filePath.contains(emptyDirPostfix))
		{
			print("Found empty directory: ", filePath);
			continue;
		}

		StreamFile file(filePath, QIODevice::ReadOnly);

		print("Reading file: ", filePath);

		while (!file.AtEnd())
		{
			print("Reading for chunk: ", currentChunk);

			auto readed = file.ReadRawData(chunkBuff.GetData(), shouldRead, true);

			if (shouldRead < readed)
			{
				throw std::runtime_error(QString("Bug? Readed more than expected. Current chunk: %1. Extra bytes: %2")
					.arg(currentChunk).arg(-shouldRead).toStdString());
			}

			shouldRead = shouldRead - readed;

			if (shouldRead == 0)
			{
				saveChunk();
			}
		}
	}

	if (shouldRead != 0)
	{
		print("Saving last chunk");
		saveChunk();
	}

	print("Saving main hash");
	resFile.WriteInArea("MainHash", mainHashAlg.result());
	print("Saved");
}

void BackupManager::RestoreData(const QString& profileName)
{
	const auto& profile = GetProfile(profileName);

	const auto resFilePath = profile.GetBackupFilePath();
	const auto emptyDirPostfix = profile.GetEmptyDirPostfix();

	StreamFile resFile(resFilePath, QIODevice::ReadOnly);

	QCryptographicHash hashAlg(QCryptographicHash::Md5);
	QCryptographicHash mainHashAlg(QCryptographicHash::Md5);

	QString prefix;
	resFile >> prefix;

	if (prefix != backupPrefix)
	{
		throw std::runtime_error(QString("It's not a backup file").toStdString());
	}

	qint32 compressionLevel;
	quint64 chunkSize;
	quint64 chunkCount;

	print("Reading header");
	//reading header.
	{
		QByteArray hash, headerData;

		resFile >> hash >> headerData;

		hashAlg.addData(headerData);

		if (hashAlg.result() != hash)
		{
			throw std::runtime_error("The header is broken");
		}

		hashAlg.reset();
		mainHashAlg.addData(hash);

		QString profileName;
		QString backupFilePath;

		StreamBuffer headerIn(headerData, QIODevice::ReadOnly);

		headerIn >> profileName >> compressionLevel >> backupFilePath
			>> chunkSize >> chunkCount;

		if (profileName != profile.GetProfileName())
		{
			throw std::runtime_error(QString("Expected profile '%1', not '%2'")
				.arg(profile.GetProfileName()).arg(profileName).toStdString());
		}

		if (compressionLevel != profile.GetCompressionLevel())
		{
			print(QString("Warning: Expected compression '%1', not '%2'")
				.arg(profile.GetCompressionLevel()).arg(compressionLevel));
		}

		if (backupFilePath != profile.GetBackupFilePath())
		{
			print(QString("Warning: Expected backup path '%1', not '%2'")
				.arg(profile.GetBackupFilePath()).arg(backupFilePath));
		}

		if (chunkSize != profile.GetChunkSize() * BytesIn1Mb)
		{
			print(QString("Warning: Expected chunk size '%1', not '%2'")
				.arg(profile.GetChunkSize()).arg(chunkSize));
		}
	}

	QByteArray mainHash;

	resFile.ReadRawData(mainHash, BytesInMd5, false);

	struct FileInfo
	{
		QString Path;
		quint64 Offset;
		quint64 Size;
	};

	QList<FileInfo> filesInfo;

	print("Reading data header");
	// reads data header.
	{
		QByteArray hash, headerData;

		resFile >> hash >> headerData;

		hashAlg.addData(headerData);

		if (hashAlg.result() != hash)
		{
			throw std::runtime_error("The data header is broken");
		}

		hashAlg.reset();
		mainHashAlg.addData(hash);

		StreamBuffer buffer(headerData, QIODevice::ReadOnly);

		quint64 length;
		buffer >> length;

		for (quint64 i = 0; i < length; ++i)
		{
			FileInfo info;
			buffer >> info.Path >> info.Offset >> info.Size;
			filesInfo.push_back(info);
		}
	}

	QString dataPrefix;
	resFile >> dataPrefix;

	if (backupDataPrefix != dataPrefix)
	{
		throw std::runtime_error("Expected backup data beginning.");
	}

	print("Reading data");

	quint64 currentFile = 0;
	quint64 currentChunk = 0;
	QByteArray fileData;
	quint64 chunkNumber;
	quint64 currentDataOffset = 0;

	QByteArray accData;

	auto loadNextChunk = [&]()
	{
		print(QString("Loading chunk %1").arg(currentChunk));

		if (currentChunk >= chunkCount)
		{
			throw std::runtime_error(QString("Invalid chunk number '%1'. Chunk count: '%2'")
				.arg(currentChunk).arg(chunkCount).toStdString());
		}

		QByteArray hash, tempData;

		resFile >> hash;
		resFile.ReadRawData(tempData, chunkSize + sizeof(chunkNumber), false);

		hashAlg.addData(tempData);

		if (hashAlg.result() != hash)
		{
			throw std::runtime_error(QString("The chunk %1 is broken")
				.arg(currentChunk).toStdString());
		}

		hashAlg.reset();
		mainHashAlg.addData(hash);

		StreamBuffer chunkBuff(tempData, QIODevice::ReadOnly);

		chunkBuff >> chunkNumber;

		if (chunkNumber != currentChunk)
		{
			throw std::runtime_error(QString("Warning: Expected chunk number '%1', not '%2'")
				.arg(currentChunk).arg(chunkNumber).toStdString());
		}

		chunkBuff.ReadRawData(accData, chunkSize, true);
		++currentChunk;
	};

	loadNextChunk();

	for (; currentFile < filesInfo.length(); ++currentFile)
	{
		if (filesInfo[currentFile].Path.contains(emptyDirPostfix))
		{
			StreamFile::MakePathToFile(filesInfo[currentFile].Path);

			continue;
		}

		StreamFile fileOut(filesInfo[currentFile].Path, QIODevice::WriteOnly);

		quint64 expectedBytes = filesInfo[currentFile].Size;

		print(QString("Writing file %1: %2")
			.arg(currentFile).arg(filesInfo[currentFile].Path));

		Q_ASSERT(currentDataOffset == filesInfo[currentFile].Offset);

		while (true)
		{
			if (expectedBytes < accData.length()) // chunk has more bytes that needed.
			{
				StreamBuffer accBuff(std::move(accData), QIODevice::ReadOnly);

				QByteArray tempData;

				expectedBytes -= accBuff.ReadRawData(tempData, expectedBytes, false);

				Q_ASSERT(expectedBytes == 0);

				fileOut.WriteRawData(tempData);
				currentDataOffset += tempData.length();

				accData = accBuff.GetRest();
				break;
			}
			else if (expectedBytes > accData.length()) // not enough bytes to finish a file.
			{
				fileOut.WriteRawData(accData);
				expectedBytes -= accData.length();
				currentDataOffset += accData.length();

				accData.clear();
				loadNextChunk();
				continue;
			}
			else // Perfect! or it's the last chunk.
			{
				fileOut.WriteRawData(accData);
				currentDataOffset += accData.length();

				accData.clear();
				
				if (currentFile < (filesInfo.length() - 1))
				{
					loadNextChunk();
				}

				break;
			}
		}
	}

	if (mainHash != mainHashAlg.result())
	{
		throw std::runtime_error("Backup file is broken! Main hash error.");
	}
}

quint64 BackupManager::CalculateFileSize(const QString& file)
{
	QFileInfo info(file);
	return info.size();
}

quint64 BackupManager::CalculateFilesSize(const QStringList& files)
{
	quint64 total = 0;

	for (const auto& file : files)
	{
		total += CalculateFileSize(file);
	}

	return total;
}

} // namespace BackupManager
