#include "ProfilesFolder.h"

void CheckProfilesFolder()
{
	QDir dir("Profiles");

	if (!dir.exists()) 
	{
		dir.mkpath(".");
	}
}