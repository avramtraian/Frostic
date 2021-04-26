#include "BuildWindows.h"

#include <filesystem>

namespace Frostic {

	bool BuildWindows::CopyFiles(const std::string& destination)
	{
		FE_CORE_INFO("Copying!");

#ifdef FE_PLATFORM_WINDOWS
		if (!std::filesystem::exists(destination + "Frostic.sln"))
		{
			std::filesystem::copy("C:/dev/Frostic-dev", destination, std::filesystem::copy_options::recursive);
			std::system((destination + "/scripts/Win-BuildRuntimeProject.bat").c_str());
		}
		else
		{
			FE_CORE_ERROR("Path already exists!");
		}
#endif

		return true;
	}

	BuildWindows BuildWindows::s_Instance = BuildWindows();

}