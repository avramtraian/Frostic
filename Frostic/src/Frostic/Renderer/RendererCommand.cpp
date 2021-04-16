#include "fepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Frostic {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();

}