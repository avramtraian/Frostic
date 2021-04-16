#include "fepch.h"
#include "ScriptManager.h"

#include "Frostic/Scene/ScriptableEntity.h"

#include "ScriptsHeaders.h"

/////////////////////////////////////////////////////////////////////
/////////////***  THIS CODE IS EXTREMLY SENSITIVE  ***///////////////
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
/////***  The editor has built-in tools to modify this file  ***/////
///////////***  You should never touch this file EVER  ***///////////
/////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////***  If you want to modify something, DO NOT CHANGE ANYTHING at the format of the SWITCH statement  ***//////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Frostic {

	std::unordered_map<uint64_t, ScriptManager::Data> ScriptManager::m_Scripts;

	auto ScriptManager::CreateInstantiateScriptByID(uint64_t id)->ScriptableEntity* (*)()
	{
		ScriptableEntity* (*function)() = []() {return static_cast<ScriptableEntity*>(nullptr); };

		switch (id)
		{
			case 9252044:
				function = []() { return static_cast<ScriptableEntity*>(new PlayerScript()); };
				break;
			default:
				FE_CORE_WARN("Invalid ID: {0}", id);
				break;
		}

		return function;
	}

}