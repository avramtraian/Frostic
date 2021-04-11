#pragma once

#include "Frostic/Core/Core.h"
#include "Frostic/Core/Log.h"
#include "Frostic/Scene/Scene.h"
#include "Frostic/Scene/Entity.h"

#include "Frostic/Assets/AssetLibrary.h"

namespace Frostic {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);
		void SetSelectionContext(Entity selectionContext);
		void SetSelectionContextFromID(int id);

		void OnImGuiRender();

		Entity GetSelectedEntity() const { return m_SelectionContext; }

		template<typename T, typename F>
		static void ForEachWhoHas(Ref<Scene> scene, F function)
		{
			auto view = scene->m_Registry.view<T>();
			for (auto entity : view)
				function(entity);
		}
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};

}