#include "pch.h"

#include "ConfigManager.h"

namespace BackupManager
{

ConfigManager::ConfigManager(const QString& filePath)
{
	Load(filePath, false);
}

bool ConfigManager::Contains(const QString& key, const bool valueRequired) const
{
	if (!keyValueSettings_.contains(key))
	{
		return false;
	}

	if (!valueRequired)
	{
		return true;
	}

	const auto val = keyValueSettings_[key];

	return val.length() > 0;
}

QString ConfigManager::GetValue(const QString& key) const
{
	if (!Contains(key, true))
	{
		throw std::runtime_error(QString("Key '%1' was not found").arg(key).toStdString());
	}

	return keyValueSettings_[key];
}

QString ConfigManager::GetValue(const QString& key, const QString& defaultVal) const
{
	if (!Contains(key, true))
	{
		return defaultVal;
	}

	return keyValueSettings_[key];
}

void ConfigManager::SetValue(const QString& key, const QString& value)
{
	keyValueSettings_[key] = value;
}

void ConfigManager::Load(const QString& filePath, const bool clear)
{
	if (clear)
	{
		keyValueSettings_.clear();
	}

	QFile file(filePath);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		throw std::runtime_error(QString("Can't open '%1' file to load config.")
			.arg(filePath).toStdString());
	}

	QTextStream in(&file);

	while (!in.atEnd())
	{
		auto line = in.readLine();

		if (line.length() == 0 || line[0] == ' ' || line[0] == '\t' || line[0] == '#')
		{
			continue;
		}

		const auto index = line.indexOf(' ');

		const QString key = line.mid(0, index);
		QString val;

		// if has a space and has characters after it.
		if (index != -1 && index < (line.length() - 1))
		{
			val = line.mid(index + 1);
		}

		keyValueSettings_[key] = val;
	}
}

void ConfigManager::Save(const QString& filePath) const
{
	QFile file(filePath);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		throw std::runtime_error(QString("Can't open '%1' file to save config.")
			.arg(filePath).toStdString());
	}

	QTextStream out(&file);

	out << "# Config section start.\n";
	auto i = keyValueSettings_.constBegin();
	while (i != keyValueSettings_.constEnd())
	{
		out << i.key() << " " << i.value() << "\n";
		++i;
	}
	out << "# Config section end.\n";

	out.flush();
}

} // namespace BackupManager
