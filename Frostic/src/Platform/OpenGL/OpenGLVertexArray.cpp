#include "fepch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Frostic {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Frostic::ShaderDataType::Float:     return GL_FLOAT;
			case Frostic::ShaderDataType::Float2:    return GL_FLOAT;
			case Frostic::ShaderDataType::Float3:    return GL_FLOAT;
			case Frostic::ShaderDataType::Float4:    return GL_FLOAT;
			case Frostic::ShaderDataType::Mat3:      return GL_FLOAT;
			case Frostic::ShaderDataType::Mat4:      return GL_FLOAT;
			case Frostic::ShaderDataType::Int:       return GL_INT;
			case Frostic::ShaderDataType::Int2:      return GL_INT;
			case Frostic::ShaderDataType::Int3:      return GL_INT;
			case Frostic::ShaderDataType::Int4:      return GL_INT;
			case Frostic::ShaderDataType::Bool:      return GL_BOOL;
		}

		FE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		FE_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		FE_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		FE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		FE_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		FE_PROFILE_FUNCTION();

		FE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case Frostic::ShaderDataType::Float:
			case Frostic::ShaderDataType::Float2:
			case Frostic::ShaderDataType::Float3:
			case Frostic::ShaderDataType::Float4:
			case Frostic::ShaderDataType::Bool:
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					vertexBuffer->GetLayout().GetStride(),
					(const void*)element.Offset);
				index++;
				break;
			case Frostic::ShaderDataType::Mat3:
			case Frostic::ShaderDataType::Mat4:
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					vertexBuffer->GetLayout().GetStride(),
					(const void*)element.Offset);
				index++;
				break;
			case Frostic::ShaderDataType::Int:
			case Frostic::ShaderDataType::Int2:
			case Frostic::ShaderDataType::Int4:
			case Frostic::ShaderDataType::Int3:
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index, element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					vertexBuffer->GetLayout().GetStride(),
					(const void*)element.Offset);
				index++;
				break;
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(Ref<IndexBuffer>& indexBuffer)
	{
		FE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}