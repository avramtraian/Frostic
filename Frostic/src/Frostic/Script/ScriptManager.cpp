#include "frpch.h"
#include "ScriptManager.h"

#include "Frostic/Scene/ScriptableEntity.h"

#include "ScriptsHeaders.h"

namespace Frostic {

	std::unordered_map<uint64_t, std::string> ScriptManager::m_Scripts;

	auto ScriptManager::CreateInstantiateScriptByID(uint64_t id) -> ScriptableEntity*(*)()
	{
		ScriptableEntity* (*function)();

		switch (id)
		{
			case 758943:
				function = []() { return static_cast<ScriptableEntity*>(new MovementScript()); };
				break;
			case 759834:
				function = []() { return static_cast<ScriptableEntity*>(new ScriptedEntity()); };
				break;
			case 9252044:
				function = []() { return static_cast<ScriptableEntity*>(new PlayerScript()); };
				break;
			default:
				FE_CORE_ASSERT(false, "Invalid ID!");
				break;
		}

		return function;
	}

}