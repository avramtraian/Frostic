#pragma once

#include "Entity.h"
#include <vector>

#define NULL_NATIVE_SCRIPT 0

#define FE_SCRIPT_CLASS(scriptID) { SetScriptID(scriptID); }

#define FE_DATA_PROPERTY(data, label, type) { \
	_m_Properties.emplace_back((void*)&data, label, PropertyType::Data, type); \
}

#define FE_DATA_STRUCTURE_PROPERTY(data, label, type, rawType) { \
	_m_Properties.emplace_back((void*)&data, label, PropertyType::DataStructure, rawType, type); \
}

#define FE_SCRIPT_PROPERTY(data, label, type) { \
	_PropertyData& property = _m_Properties.emplace_back((void*)&data, label, PropertyType::Script); \
	{ \
		type script = type(); \
		property.m_ScriptID = script.GetScriptID(); \
	} \
}

#define FE_ENTITY_REFERENCE_PROPERTY(data, label) { \
	_m_Properties.emplace_back((void*)&data, label, PropertyType::EntityReference); \
}

namespace Frostic {

	enum class PropertyType
	{
		Data,
		DataStructure,
		Script,
		EntityReference
	};

	enum class DataType
	{
		None = 0,
		UINT8_T,
		UINT16_T,
		UINT32_T,
		UINT64_T,
		INT,
		FLOAT
	};

	enum class DataStructureType
	{
		None = 0,
		FEVectorType = 1,
		FEArrayType = 2,
		FEStringType = 3
	};

	class ScriptableEntity
	{
	public:
		ScriptableEntity() = default;
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		struct _PropertyData
		{
		public:
			_PropertyData() = default;
			_PropertyData(void* data, const std::string& label, PropertyType propertyType, DataType dataType = DataType::None, DataStructureType dataStructureType = DataStructureType::None)
				: m_Data(data), m_Label(label), m_PropertyType(propertyType), m_DataType(dataType), m_DataStructureType(dataStructureType) {}
		public:
			void* m_Data = nullptr;
			std::string m_Label = std::string();
			PropertyType m_PropertyType;
			DataType m_DataType = DataType::None;
			DataStructureType m_DataStructureType = DataStructureType::None;
			uint64_t m_EntityUUID = 0;
			uint64_t m_ScriptID = 0;
		};
		std::vector<_PropertyData> _m_Properties;

		const uint64_t GetScriptID() const { return m_ScriptID; }
		Entity GetEntity() const { return m_Entity; }
		const uint64_t GetEntityUUID() const { return m_EntityUUID; }

	protected:
		virtual void Begin() {}
		virtual void OnDestroy() {}
		virtual void Tick(Timestep ts) {}

		virtual void PushProperties() {}

		void SetScriptID(uint64_t newScriptID) { m_ScriptID = newScriptID; }
	private:
		Entity m_Entity;
		uint64_t m_EntityUUID = 0;
		uint64_t m_ScriptID = 0;
		friend class Scene;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	 };

}