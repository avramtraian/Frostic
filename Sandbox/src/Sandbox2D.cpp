#include "Sandbox2D.h"
#include "Imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1600.0f / 900.0f)
{
}

void Sandbox2D::OnAttach()
{
	m_SquareVA = Frostic::VertexArray::Create();

	float squareVertices[4 * 3] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Frostic::Ref<Frostic::VertexBuffer> squareVB;
	squareVB.reset(Frostic::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	Frostic::BufferLayout squareLayout = {
		{ Frostic::ShaderDataType::Float3, "a_Position" },
	};
	squareVB->SetLayout(squareLayout);

	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Frostic::Ref<Frostic::IndexBuffer> squareIB;
	squareIB.reset(Frostic::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);
	
	m_FlatColorShader = Frostic::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Frostic::Timestep ts)
{
	// Updating
	m_CameraController.OnUpdate(ts);

	// Rendering
	Frostic::RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 1 });
	Frostic::RenderCommand::Clear();

	Frostic::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Frostic::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Frostic::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Frostic::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Frostic::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Frostic::Event& e)
{
	m_CameraController.OnEvent(e);
}