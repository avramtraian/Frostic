#pragma once

#include "Frostic/Scene/ScriptableEntity.h"
#include "Frostic/Scene/Components.h"

namespace Frostic {

	class ScriptManager
	{
	public:
		struct ScriptData
		{
		public:
			ScriptData() = default;

			template<typename T>
			void Create()
			{
				InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
				DestroyScript[](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
			}

			ScriptableEntity* (InstantiateScript)() { return nullptr; }
			void (DestroyScript)(NativeScriptComponent*) {}
		};
	public:
		template<typename T>
		static void AddScript()
		{
			T script = T();
			m_Scripts[script.GetScriptID()] = ScriptData();
			m_Scripts[script.GetScriptID()].Create<T>();
		}
		static ScriptData GetScriptData(uint64_t id)
		{
			FE_CORE_ASSERT(m_Scripts.find(id) != m_Scripts.end(), "Invalid Script ID!");
			return m_Scripts[id];
		}
	private:
		static std::unordered_map<uint64_t, ScriptData> m_Scripts;
	};

}