#pragma once
#include <Frostic.h>

#include "Panels/SceneHierarchyPanel.h"

namespace Frostic {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		uint32_t m_FPS = 0;
		OrthographicCameraController m_CameraController;

		Ref<Framebuffer> m_Framebuffer;

		EditorCamera m_EditorCamera;

		Ref<Scene> m_ActiveScene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;

		Ref<Texture2D> m_CheckerboardTexture;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = {0.0f, 0.0f};

		glm::vec4 m_Color = { 0.2f, 0.3f, 0.8f, 1.0f };

		// Panels
		SceneHierarchyPanel m_HierarchyPanel;
	};

}