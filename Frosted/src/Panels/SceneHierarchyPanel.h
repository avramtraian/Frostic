#pragma once

#include "Frostic/Core/Core.h"
#include "Frostic/Core/Log.h"
#include "Frostic/Scene/Scene.h"
#include "Frostic/Scene/Entity.h"

namespace Frostic {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void SetSelectionContextFromID(const uint32_t id);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}