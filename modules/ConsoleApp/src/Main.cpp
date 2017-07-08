#include "BackupManager.h"
#include <iostream>

int main(int argc, char* argv[])
{
	BackupManager::BackupManager backuper;

	QTextStream in(stdin);
	QTextStream out(stdout);

	QString defProfile;

	while (true)
	{
		try
		{
			out << "$: ";
			out.flush();
			QString command;
			in >> command;

			if (command == "q" || command == "quit")
			{
				break;
			}

			if (command == "def")
			{
				in >> defProfile;

				continue;
			}

			if (command == "def_rem")
			{
				defProfile.clear();

				continue;
			}

			if (command == "add")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				backuper.CreateProfile(name);

				continue;
			}

			if (command == "view_all")
			{
				for (const auto& pr : backuper.GetProfiles())
				{
					out << "+++++++++++++++++++++++++++\n";
					out << "Name: " << pr.GetProfileName() << "\n";
					out << "Chunk size: " << pr.GetChunkSize() << "\n";
					out << "Compression: " << pr.GetCompressionLevel() << "\n";
					out << "Result file: " << pr.GetBackupFilePath() << "\n";
					out << "Files: \n";

					for (const auto& file : pr.GetBackupedFilesPath())
					{
						out << "\t" << file << "\n";
					}

					out << "---------------------------\n";

					out.flush();
				}

				continue;
			}

			if (command == "prof_compr")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				int level;
				in >> level;

				backuper.ModifyProfile(name).SetCompressionLevel(level);

				continue;
			}

			if (command == "prof_chunk")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				int size;
				in >> size;

				backuper.ModifyProfile(name).SetChunkSize(size);

				continue;
			}

			if (command == "prof_res")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				QString result;
				in >> result;

				backuper.ModifyProfile(name).SetBackupFilePath(result);

				continue;
			}

			if (command == "prof_file")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				QString file;
				in >> file;

				backuper.ModifyProfile(name).AddBackupedFilePath(file);

				continue;
			}

			if (command == "prof_dir")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				QString dir;
				in >> dir;

				backuper.ModifyProfile(name).AddBackupedFolderPath(dir);

				continue;
			}

			if (command == "save")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				backuper.GetProfile(name).SaveProfile();

				continue;
			}

			if (command == "load")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				backuper.CreateProfile(name);

				backuper.ModifyProfile(name).LoadProfile();

				continue;
			}

			if (command == "backup")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				backuper.BackupData(name);

				out << "Success\n";
				out.flush();

				continue;
			}

			if (command == "restore")
			{
				QString name;

				if (defProfile.isEmpty())
					in >> name;
				else
					name = defProfile;

				backuper.RestoreData(name);

				out << "Success\n";
				out.flush();

				continue;
			}

			out << "Unknown command\n";
			out.flush();
		}
		catch (const std::exception& e)
		{
			out << "Error: " << e.what() << "\n";
			out.flush();
		}
	}
}