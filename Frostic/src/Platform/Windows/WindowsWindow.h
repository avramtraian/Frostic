#pragma once

#include "Frostic/Core/Window.h"

#include <GLFW/glfw3.h>

#include "Frostic/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Frostic {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void* GetNativeWindow() const override { return m_Window; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		void SetProperties(WindowProps& props) override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool bVSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}