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
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMousePressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveScene();
		void SaveSceneAs();
	private:
		uint32_t m_FPS = 0;
		OrthographicCameraController m_CameraController;

		Ref<Framebuffer> m_Framebuffer;
		Ref<Scene> m_ActiveScene;
		EditorCamera m_EditorCamera;

		std::string saveFilepath = std::string();

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = {0.0f, 0.0f};
		glm::vec2 m_ViewportBounds[2]; 

		int m_GizmoType = -1;

		// Panels
		SceneHierarchyPanel m_HierarchyPanel;
	};

}