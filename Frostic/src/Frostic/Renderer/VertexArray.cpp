#include "frpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Frostic {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:     FR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:   return std::make_shared<OpenGLVertexArray>();
		}

		FR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}