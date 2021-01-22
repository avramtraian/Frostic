#pragma once

#include "Core.h"

#include "Window.h"
#include "Frostic/Core/LayerStack.h"
#include "Frostic/Events/Event.h"
#include "Frostic/Events/ApplicationEvent.h"

#include "Frostic/Core/Timestep.h"

#include "Frostic/ImGui/ImGuiLayer.h"

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
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		bool m_Running = true;
		bool m_Minimized = false;

		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();

}