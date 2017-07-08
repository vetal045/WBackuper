#pragma once

namespace BackupManager
{

//
// Declaration of the StreamDevice class.
//

class StreamDevice
{
	//
	// Construction and destruction.
	//
public:
	//! Constructor.
	StreamDevice(QIODevice& device);
	//! Destructor.
	virtual ~StreamDevice() = 0 
	{
	}

	//
	// Public interface.
	//
public:
	//! Reserves an area for future.
	void Reserve(const QString& name, quint64 size);
	//! Writes the bytes into reserved area.
	void WriteInArea(const QString& name, const QByteArray& data);

	//! Writes raw data.
	void WriteRawData(const QByteArray& data);
	//! Reads raw data.
	quint64 ReadRawData(QByteArray& in, const quint64 length, const bool append);

	//! Returns true if stream at the end of the file.
	bool AtEnd() const;

	//! Returns device size.
	quint64 GetSize() const;

	//! Output operator.
	template<typename T>
	StreamDevice& operator<< (const T& val);

	//! Input operator.
	template<typename T>
	StreamDevice& operator >> (T& val);

	//
	// Protected methods.
	//
protected:
	//! Flushes the device.
	virtual void Flush()
	{
	}

	//! Initialized current object.
	void Initialize();

	//
	// Private data members.
	//
private:
	//! Device.
	QIODevice& device_;
	//! Device stream.
	QDataStream stream_;
	//! Reserved areas.
	QMap < QString, QPair<quint64, quint64>> reserveAreas_;
};

//
// StreamDevice implementation.
//

template<typename T>
StreamDevice& StreamDevice::operator<< (const T& val)
{
	stream_ << val;

	Flush();

	return *this;
}

template<typename T>
StreamDevice& StreamDevice::operator >> (T& val)
{
	stream_ >> val;

	return *this;
}

//
// Declaration of the StreamFile class.
//

class StreamFile : public StreamDevice
{
	//
	// Construction and destruction
	//
public:
	//! Constructor.
	StreamFile(const QString& path, const QIODevice::OpenModeFlag flags);

	//
	// Public interface.
	//
public:
	//! Creates directories required to file.
	static void MakePathToFile(const QString& filePath);

	//
	// Protected methods.
	//
protected:
	//! Flushes the device.
	virtual void Flush() override;

	//
	// Private data members.
	//
private:
	//! File device.
	QFile file_;
};

//
// Declaration of the StreamArray class.
//

class StreamBuffer : public StreamDevice
{
	//
	// Construction and destruction
	//
public:
	//! Constructor.
	StreamBuffer(QIODevice::OpenMode flags);
	//! Constructor.
	StreamBuffer(const QByteArray& arr, QIODevice::OpenMode flags);
	//! Constructor.
	StreamBuffer(QByteArray&& arr, QIODevice::OpenMode flags);

	//
	// Public interface.
	//
public:
	//! Returns a reference to the buffer.
	QByteArray& GetData();
	//! Returns the buffer.
	QByteArray MoveData();
	//! Returns the unreaded bytes.
	QByteArray GetRest();
	//! Clears buffer.
	void Clear();

	//
	// Private data members.
	//
private:
	//! File device.
	QByteArray array_;
	//! Array device.
	QBuffer buffer_;
};

} // namespace BackupManager
