#include "pch.h"

#include "StreamDevice.h"

namespace BackupManager
{

StreamDevice::StreamDevice(QIODevice& device)
	: device_(device)
{

}

void StreamDevice::Initialize()
{
	stream_.setDevice(&device_);
	stream_.setVersion(QDataStream::Qt_5_8);
}

void StreamDevice::Reserve(const QString& name, quint64 size)
{
	if (reserveAreas_.contains(name))
	{
		throw std::runtime_error(QString("Area '%1' already exists").arg(name).toStdString());
	}

	reserveAreas_[name] = QPair<qint64, quint64>(device_.pos(), size);
	
	if (!device_.seek(device_.pos() + size))
	{
		throw std::runtime_error(QString("Can't move position in device").toStdString());
	}
}

void StreamDevice::WriteInArea(const QString& name, const QByteArray& data)
{
	if (!reserveAreas_.contains(name))
	{
		throw std::runtime_error(QString("Area '%1' does not exist").arg(name).toStdString());
	}

	const auto& area = reserveAreas_[name];

	if (area.second != data.length())
	{
		throw std::runtime_error(QString("Invalid data size for area '%1'. Expected '%2', got '%3'")
			.arg(name).arg(area.second).arg(data.length()).toStdString());
	}

	auto prevPos = device_.pos();

	device_.seek(area.first);
	WriteRawData(data);

	device_.seek(prevPos);

	Flush();
}

void StreamDevice::WriteRawData(const QByteArray& data)
{
	stream_.writeRawData(data.data(), data.length());

	Flush();
}

quint64 StreamDevice::ReadRawData(QByteArray& in, const quint64 length, const bool append)
{
	if (!append)
	{
		in.clear();
	}

	auto newLength = GetSize();
	if (newLength > length)
	{
		newLength = length;
	}

	auto oldLength = in.length();

	in.resize(newLength + oldLength);
	auto readed = stream_.readRawData(in.data() + oldLength, newLength);

	in.resize(oldLength + readed);

	return readed;
}

bool StreamDevice::AtEnd() const
{
	return device_.atEnd();
}

quint64 StreamDevice::GetSize() const
{
	return device_.size();
}

StreamFile::StreamFile(const QString& path, const QIODevice::OpenModeFlag flags)
	: StreamDevice(file_)
	, file_(path)
{
	MakePathToFile(path);

	if (!file_.open(flags))
	{
		throw std::runtime_error(QString("File '%1' is not opened").arg(path).toStdString());
	}

	Initialize();
}

void StreamFile::MakePathToFile(const QString& filePath)
{
	QFileInfo fi(filePath);

	auto dir = fi.absoluteDir();
	dir.mkpath(dir.path());
}

void StreamFile::Flush()
{
	file_.flush();
}

StreamBuffer::StreamBuffer(QIODevice::OpenMode flags)
	: StreamDevice(buffer_)
	, buffer_(&array_)
{
	if (!buffer_.open(flags))
	{
		throw std::runtime_error(QString("Buffer is not opened").toStdString());
	}

	Initialize();
}

StreamBuffer::StreamBuffer(const QByteArray& arr, QIODevice::OpenMode flags)
	: StreamDevice(buffer_)
	, array_(arr)
	, buffer_(&array_)
{
	if (!buffer_.open(flags))
	{
		throw std::runtime_error(QString("Buffer is not opened").toStdString());
	}

	Initialize();
}

StreamBuffer::StreamBuffer(QByteArray&& arr, QIODevice::OpenMode flags)
	: StreamDevice(buffer_)
	, array_(std::move(arr))
	, buffer_(&array_)
{
	if (!buffer_.open(flags))
	{
		throw std::runtime_error(QString("Buffer is not opened").toStdString());
	}

	Initialize();
}

QByteArray& StreamBuffer::GetData()
{
	return array_;
}

QByteArray StreamBuffer::MoveData()
{
	return std::move(array_);
}

QByteArray StreamBuffer::GetRest()
{
	return array_.remove(0, buffer_.pos());
}

void StreamBuffer::Clear()
{
	array_.clear();
	buffer_.seek(0);
}

} // namespace BackupManager
