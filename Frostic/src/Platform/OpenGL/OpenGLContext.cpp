#include "frpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Frostic {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		FR_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FR_CORE_ASSERT(status, "Failed to initialize Glad!");

		FR_CORE_INFO("OpenGL Info: ");
		FR_CORE_INFO("   Vendor: {0}", glGetString(GL_VENDOR));
		FR_CORE_INFO("   Renderer: {0}", glGetString(GL_RENDERER));
		FR_CORE_INFO("   Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}

}