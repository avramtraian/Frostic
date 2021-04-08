#pragma once

#include "Entity.h"
#include <vector>

#define FE_DATA_PROPERTY(data, label, type) { \
	m_DataReferences.emplace_back((void*)&data, label, type); \
}

#define FE_DATA_STRUCTURE_PROPERTY(data, label, type, rawType) { \
	m_DataStructureReferences.emplace_back((void*)&data, label, type, rawType); \
}

#define FE_ENTITY_REFERENCE_PROPERTY(data, label) { \
	m_EntityReferences.emplace_back(&data, label); \
}

namespace Frostic {

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
		virtual ~ScriptableEntity() = default;

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

		struct PropertyEntity 
		{ 
			PropertyEntity() = default; 
			PropertyEntity(Entity* data, const std::string& label)
				: m_Data(data), m_Label(label) {}
			Entity* m_Data = nullptr; 
			uint64_t UUID = 0;
			std::string m_Label = std::string(); 
		}; 
		struct PropertyData 
		{ 
			PropertyData() = default; 
			PropertyData(void* data, const std::string& label, DataType dataType) 
			: m_Data(data), m_Label(label), m_DataType(dataType) {} 
			void* m_Data = nullptr; 
			DataType m_DataType; 
			std::string m_Label = std::string(); 
		}; 
		struct PropertyDataStructure 
		{ 
			PropertyDataStructure() = default; 
			PropertyDataStructure(void* data, const std::string& label, DataStructureType dataStructureType, DataType dataType) 
			: m_Data(data), m_Label(label), m_DataStructureType(dataStructureType), m_DataType(dataType) {} 
			void* m_Data = nullptr; 
			std::string m_Label = std::string(); 
			DataStructureType m_DataStructureType; 
			DataType m_DataType; 
		}; 
		std::vector<PropertyEntity> m_EntityReferences; 
		std::vector<PropertyData> m_DataReferences; 
		std::vector<PropertyDataStructure> m_DataStructureReferences;

	protected:
		virtual void Begin() {}
		virtual void OnDestroy() {}
		virtual void Tick(Timestep ts) {}

		virtual void PushProperties() {}
	private:
		Entity m_Entity;
		uint64_t m_EntityUUID = 0;
		friend class Scene;
		friend class SceneHierarchyPanel;
	 };

}