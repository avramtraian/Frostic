#include "frpch.h"
#include "Entity.h"

namespace Frostic {
	
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene) 
	{
	}

}