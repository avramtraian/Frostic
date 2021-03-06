#include <Frostic.h>
#include <Frostic/Core/EntryPoint.h>

#include "Imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D.h"

class ExampleLayer : public Frostic::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1600.0f / 900.0f, true)
	{
		m_VertexArray = Frostic::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
		};

		Frostic::Ref<Frostic::VertexBuffer> vertexBuffer;
		vertexBuffer = Frostic::VertexBuffer::Create(vertices, sizeof(vertices));
		Frostic::BufferLayout layout = {
			{ Frostic::ShaderDataType::Float3, "a_Position" },
			{ Frostic::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Frostic::Ref<Frostic::IndexBuffer> indexBuffer;
		indexBuffer = Frostic::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Frostic::VertexArray::Create();

		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Frostic::Ref<Frostic::VertexBuffer> squareVB;
		squareVB = Frostic::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		Frostic::BufferLayout squareLayout = {
			{ Frostic::ShaderDataType::Float3, "a_Position" },
			{ Frostic::ShaderDataType::Float2, "a_TexCoords" }
		};
		squareVB->SetLayout(squareLayout);

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Frostic::Ref<Frostic::IndexBuffer> squareIB;
		squareIB = Frostic::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position; 
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
		m_FlatColorShader = Frostic::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		m_Texture = Frostic::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Frostic::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Frostic::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Frostic::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Frostic::Timestep ts) override
	{
		// Updating
		m_CameraController.OnUpdate(ts);

		// Rendering
		Frostic::RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 1 });
		Frostic::RenderCommand::Clear();

		Frostic::Renderer::BeginScene(m_CameraController.GetCamera());

		std::dynamic_pointer_cast<Frostic::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Frostic::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);
		
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for(int y = 0; y < 10; y++)
		{ 
			for (int x = 0; x < 10; x++)
			{
				glm::vec3 pos(x * 0.11, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Frostic::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Frostic::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Frostic::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Frostic::Renderer::Submit(m_RedShader, m_VertexArray);

		Frostic::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Frostic::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}
private:
	Frostic::ShaderLibrary m_ShaderLibrary;

	Frostic::Ref<Frostic::Shader> m_FlatColorShader;

	Frostic::Ref<Frostic::VertexArray> m_VertexArray;
	Frostic::Ref<Frostic::VertexArray> m_SquareVA;

	glm::vec3 m_SquareColor = { 0.2, 0.3, 0.8 };

	Frostic::Ref<Frostic::Texture2D> m_Texture, m_ChernoLogoTexture;

	Frostic::OrthographicCameraController m_CameraController;
};

class Sandbox : public Frostic::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox()
	{

	}
};

Frostic::Application* Frostic::CreateApplication()
{
	return new Sandbox();
}