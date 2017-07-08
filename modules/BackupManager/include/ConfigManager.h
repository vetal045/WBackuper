#pragma once

namespace BackupManager
{

//
// Declaration of the ConfigManager class.
//

class ConfigManager
{
	//
	// Construction and destruction.
	//
public:
	ConfigManager() = default;
	//! Constructor.
	ConfigManager(const QString& filePath);

	//
	// Public interface.
	//
public:
	//! Returns true if the key exists.
	bool Contains(const QString& key, const bool valueRequired = false) const;
	//! Returns a value for the key or throws and error if not found.
	QString GetValue(const QString& key) const;
	//! Returns a value for the key or default value if not found.
	QString GetValue(const QString& key, const QString& defaultVal) const;

	//! Sets the key's value.
	void SetValue(const QString& key, const QString& value);

	//! Loads a config from the file.
	void Load(const QString& filePath, const bool clear);
	//! Saves a the config to the file.
	void Save(const QString& filePath) const;

	//
	// Private data members.
	//
private:
	//! Config key-value pairs.
	QMap<QString, QString> keyValueSettings_;
};

} // namespace BackupManager