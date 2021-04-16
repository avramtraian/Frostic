#pragma once

#include <string>

namespace Frostic {

	class ScriptSerializer
	{
	public:
		ScriptSerializer() = default;

		void Serialize(const std::string& filepath);
		bool Deserialize(const std::string& filepath);
	};

}