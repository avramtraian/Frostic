#pragma once

#include "Entity.h"
#include <vector>

#define NULL_NATIVE_SCRIPT 0

#define FE_SCRIPT_CLASS(scriptID) { SetScriptID(scriptID); }

#define FE_DATA_PROPERTY(data, label, type, accessM) { \
	_m_Properties.emplace_back((void*)&data, label, PropertyType::Data, accessM, type); \
}

#define FE_DATA_STRUCTURE_PROPERTY(data, label, type, rawType, accessM) { \
	_m_Properties.emplace_back((void*)&data, label, PropertyType::DataStructure, accessM, rawType, type); \
}

#define FE_SCRIPT_PROPERTY(data, label, type, accessM) { \
	_PropertyData& property = _m_Properties.emplace_back((void*)&data, label, PropertyType::Script, accessM); \
	{ \
		type script = type(); \
		property.m_ScriptID = script.GetScriptID(); \
	} \
}

#define FE_ENTITY_REFERENCE_PROPERTY(data, label, accessM) { \
	_m_Properties.emplace_back((void*)&data, label, PropertyType::EntityReference, accessM); \
}

namespace Frostic {

	enum class PropertyType : uint8_t
	{
		Data,
		DataStructure,
		Script,
		EntityReference
	};

	enum class DataType : uint8_t
	{
		None = 0,
		UINT8_T,
		UINT16_T,
		UINT32_T,
		UINT64_T,
		INT8_T,
		INT16_T,
		INT32_T,
		INT64_T,
		INT,
		FLOAT
	};

	enum class DataStructureType : uint8_t
	{
		None = 0,
		FEVectorType = 1,
		FEArrayType = 2,
		FEStringType = 3
	};

	enum class AccesModifiers : uint8_t
	{
		None = 0,
		ReadOnly,
		ReadAndWrite
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

		template<typename T>
		bool HasComponent()
		{
			return m_Entity.HasComponent<T>();
		}

		template<typename T>
		T* GetComponentPointer()
		{
			if (m_Entity.HasComponent<T>())
				return &m_Entity.GetComponent<T>();
			return nullptr;
		}

		Scene* GetScene() const { return m_Entity.GetScene(); }

		struct _PropertyData
		{
		public:
			_PropertyData() = default;
			_PropertyData(void* data, const std::string& label, PropertyType propertyType, AccesModifiers acces, DataType dataType = DataType::None, DataStructureType dataStructureType = DataStructureType::None)
				: m_Data(data), m_Label(label), m_PropertyType(propertyType), m_Acces(acces), m_DataType(dataType), m_DataStructureType(dataStructureType) {}
		public:
			void* m_Data = nullptr;
			std::string m_Label = std::string();
			PropertyType m_PropertyType;
			AccesModifiers m_Acces;
			DataType m_DataType = DataType::None;
			DataStructureType m_DataStructureType = DataStructureType::None;
			uint64_t m_EntityUUID = 0;
			uint64_t m_ScriptID = 0;
		};
		std::vector<_PropertyData> _m_Properties;

		uint64_t GetScriptID() const { return m_ScriptID; }
		Entity GetEntity() const { return m_Entity; }
		uint64_t GetEntityUUID() const { return m_EntityUUID; }

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