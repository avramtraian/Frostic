#pragma once

#include "Frostic/Scene/ScriptableEntity.h"

namespace Frostic {

	class ScriptManager
	{
	public:
		struct Data
		{
			Data() = default;
			Data(const std::string& name, const std::string& hfilepath, const std::string& cppfilepath)
				: Name(name), HFilepath(hfilepath), CPPFilepath(cppfilepath) {}

			std::string Name = std::string();
			std::string HFilepath = std::string();
			std::string CPPFilepath = std::string();
		};
	public:
		static void AddScript(uint64_t id, const std::string& name, const std::string& hfilepath, const std::string& cppfilepath)
		{
			m_Scripts[id] = Data(name, hfilepath, cppfilepath);
		}
		static void RemoveScript(uint64_t id)
		{
			m_Scripts.erase(id);
		}
		static std::string& GetNameFromID(uint64_t id)
		{
			return m_Scripts.at(id).Name;
		}
		static bool Exists(const std::string& name)
		{
			for (auto& script : m_Scripts)
			{
				if (script.second.Name == name)
					return true;
			}
			return false;
		}
		static bool Exists(uint64_t id)
		{
			return m_Scripts.find(id) != m_Scripts.end();
		}

		// The lambda takes 2 parameters : the id(uint64_t) & the name(std::string)
		template<typename F>
		static void Each(F function)
		{
			for (auto& script : m_Scripts)
				function(script.first, script.second);
		}

		static auto CreateInstantiateScriptByID(uint64_t id) -> ScriptableEntity*(*)();
	private:
		static std::unordered_map<uint64_t, Data> m_Scripts;
	};

}