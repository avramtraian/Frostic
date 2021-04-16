#include "fepch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Frostic {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		FE_CORE_ASSERT(windowHandle, "Window Handle is null!");
	}

	void OpenGLContext::Init()
	{
		FE_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FE_CORE_ASSERT(status, "Failed to initialize Glad!");

		FE_CORE_INFO("OpenGL Info: ");
		FE_CORE_INFO("   Vendor: {0}", glGetString(GL_VENDOR));
		FE_CORE_INFO("   Renderer: {0}", glGetString(GL_RENDERER));
		FE_CORE_INFO("   Version: {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		FE_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}