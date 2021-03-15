#pragma once

#include "Frostic/Core/Timestep.h"
#include "Frostic/Renderer/EditorCamera.h"

#include "Frostic/Renderer/Renderer2D.h"

#include <entt.hpp>

namespace Frostic {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		Ref<Scene> CopyScene();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		Renderer2D::SpriteSpecifications Specs;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class ToolsPanel;
		friend class SceneSerializer;
		friend class SceneManagerEditor;
		friend class SceneManagerRuntime;
	};
}