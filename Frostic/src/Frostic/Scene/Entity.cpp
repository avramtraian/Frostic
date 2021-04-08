#include "frpch.h"
#include "Entity.h"

namespace Frostic {
	
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) 
	{
	}

	Entity::Entity(const Entity& other)
	: m_Scene(other.m_Scene), m_EntityHandle(other.m_EntityHandle) 
	{
	}

}