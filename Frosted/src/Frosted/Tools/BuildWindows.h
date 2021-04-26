#pragma once

#include "Frostic/Core/Core.h"
#include "Frostic/Core/Log.h"

namespace Frostic {

	class BuildWindows
	{
	public:
		BuildWindows(const BuildWindows& other) = delete;
		static BuildWindows& Get() { return s_Instance; }

		bool CopyFiles(const std::string& destination);
	private:
		BuildWindows() = default;
		static BuildWindows s_Instance;
	};

}