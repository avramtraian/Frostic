#pragma once

#include "Frostic/Scene/ScriptableEntity.h"

namespace Frostic {

	class ScriptManager
	{
	public:
		static void AddScript(uint64_t id, const std::string& name)
		{
			m_Scripts[id] = name;
		}
		static void RemoveScript(uint64_t id)
		{
			m_Scripts.erase(id);
		}
		static std::string& GetNameFromID(uint64_t id)
		{
			return m_Scripts.at(id);
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
		static std::unordered_map<uint64_t, std::string> m_Scripts;
	};

}