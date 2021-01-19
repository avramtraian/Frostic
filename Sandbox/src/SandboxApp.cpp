#include <Frostic.h>

#include "Imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Frostic::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f), m_CameraRotation(0.0f)
	{
		m_VertexArray.reset(Frostic::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
		};

		Frostic::Ref<Frostic::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Frostic::VertexBuffer::Create(vertices, sizeof(vertices)));
		Frostic::BufferLayout layout = {
			{ Frostic::ShaderDataType::Float3, "a_Position" },
			{ Frostic::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Frostic::Ref<Frostic::IndexBuffer> indexBuffer;
		indexBuffer.reset(Frostic::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Frostic::VertexArray::Create());

		float squareVertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Frostic::Ref<Frostic::VertexBuffer> squareVB;
		squareVB.reset(Frostic::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Frostic::BufferLayout squareLayout = {
			{ Frostic::ShaderDataType::Float3, "a_Position" },
			{ Frostic::ShaderDataType::Float2, "a_TexCoords" }
		};
		squareVB->SetLayout(squareLayout);

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Frostic::Ref<Frostic::IndexBuffer> squareIB;
		squareIB.reset(Frostic::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		m_TextureShader.reset(Frostic::Shader::Create("assets/shaders/Texture.glsl"));
		m_FlatColorShader.reset(Frostic::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		m_Texture = Frostic::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Frostic::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Frostic::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Frostic::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Frostic::Timestep ts) override
	{
		if(Frostic::Input::IsKeyPressed(FR_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if(Frostic::Input::IsKeyPressed(FR_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if(Frostic::Input::IsKeyPressed(FR_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;
		else if(Frostic::Input::IsKeyPressed(FR_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;

		if (Frostic::Input::IsKeyPressed(FR_KEY_Q))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Frostic::Input::IsKeyPressed(FR_KEY_E))
			m_CameraRotation -= m_CameraRotationSpeed * ts;


		Frostic::RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 1 });
		Frostic::RenderCommand::Clear();

		m_Camera.SetPostion(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Frostic::Renderer::BeginScene(m_Camera);

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

		m_Texture->Bind();
		Frostic::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Frostic::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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

	void OnEvent(Frostic::Event& event) override
	{
		
	}
private:
	Frostic::Ref<Frostic::Shader> m_FlatColorShader, m_TextureShader;

	Frostic::Ref<Frostic::VertexArray> m_VertexArray;
	Frostic::Ref<Frostic::VertexArray> m_SquareVA;

	glm::vec3 m_SquareColor = { 0.2, 0.3, 0.8 };

	Frostic::Ref<Frostic::Texture2D> m_Texture, m_ChernoLogoTexture;

	Frostic::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraMoveSpeed = 4.0f;

	float m_CameraRotation;
	float m_CameraRotationSpeed = 30.0f;
};

class Sandbox : public Frostic::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox()
	{

	}
};

Frostic::Application* Frostic::CreateApplication()
{
	return new Sandbox();
}