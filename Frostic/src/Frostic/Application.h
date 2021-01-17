#pragma once

#include "Core.h"

#include "Window.h"
#include "Frostic/LayerStack.h"
#include "Frostic/Events/Event.h"
#include "Frostic/Events/ApplicationEvent.h"

#include "Frostic/ImGui/ImGuiLayer.h"

#include "Frostic/Renderer/Shader.h"
#include "Frostic/Renderer/Buffer.h"
#include "Frostic/Renderer/VertexArray.h"
#include "Frostic/Renderer/OrthographicCamera.h"

namespace Frostic {

	class FROSTIC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_SquareShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;
	private:
		static Application* s_Instance;
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();

}