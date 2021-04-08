#pragma once

#include "Scene.h"

#include <entt.hpp>

#define NULL_ENTITY false

namespace Frostic {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other);

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			FE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			component.EnttOwnerID = (uint32_t)m_EntityHandle;
			component.Context = m_Scene;
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			FE_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		T& GetComponent()
		{
			FE_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		T* GetComponentP()
		{
			FE_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component!");
			return &(m_Scene->m_Registry.get<T>(m_EntityHandle));
		}
		
		template<typename T>
		T& AddOrGetComponent()
		{
			if (HasComponent<T>())
				return GetComponent<T>();
			else
				return AddComponent<T>();
		}

		template<typename T> 
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}

		const bool IsValid() const { return m_Scene->m_Registry.valid(m_EntityHandle); }
		const bool Exists() const 
		{ 
			return m_Scene != nullptr && IsValid();
		}

		void Invalidate() { m_Scene = nullptr; m_EntityHandle = entt::null; }

		operator bool() const { return (m_EntityHandle != entt::null); }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& other) const { return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene; }
		bool operator!=(const Entity& other) const { return !(*this == other); }

		bool operator==(bool other) const { return other == Exists(); }
		bool operator!=(bool other) const { return other != Exists(); }
	protected:
		entt::entity m_EntityHandle{ entt::null };  
		Scene* m_Scene = nullptr;

		friend SceneHierarchyPanel;
	};

}