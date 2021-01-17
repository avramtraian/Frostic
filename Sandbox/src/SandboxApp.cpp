#include <Frostic.h>

#include "Imgui/imgui.h"

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

		std::shared_ptr<Frostic::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Frostic::VertexBuffer::Create(vertices, sizeof(vertices)));
		Frostic::BufferLayout layout = {
			{ Frostic::ShaderDataType::Float3, "a_Position" },
			{ Frostic::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Frostic::IndexBuffer> indexBuffer;
		indexBuffer.reset(Frostic::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Frostic::VertexArray::Create());

		float squareVertices[4 * 3] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Frostic::VertexBuffer> squareVB;
		squareVB.reset(Frostic::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		Frostic::BufferLayout squareLayout = {
			{ Frostic::ShaderDataType::Float3, "a_Position" }
		};
		squareVB->SetLayout(squareLayout);

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Frostic::IndexBuffer> squareIB;
		squareIB.reset(Frostic::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;
			
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core

			in vec3 v_Position;
			in vec4 v_Color;
			
			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
	
			uniform mat4 u_ViewProjection; 

			out vec3 v_Position;
			
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core

			in vec3 v_Position;
			
			layout(location = 0) out vec4 color;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(Frostic::Shader::Create(vertexSrc, fragmentSrc));
		m_SquareShader.reset(Frostic::Shader::Create(squareVertexSrc, squareFragmentSrc));
	}

	void OnUpdate(Frostic::Timestep ts) override
	{
		if(Frostic::Input::IsKeyPressed(FR_KEY_UP))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		else if(Frostic::Input::IsKeyPressed(FR_KEY_DOWN))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;

		if(Frostic::Input::IsKeyPressed(FR_KEY_RIGHT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if(Frostic::Input::IsKeyPressed(FR_KEY_LEFT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Frostic::Input::IsKeyPressed(FR_KEY_Q))
			m_CameraRotation -= m_CameraRotationSpeed * ts;
		else if (Frostic::Input::IsKeyPressed(FR_KEY_E))
			m_CameraRotation += m_CameraRotationSpeed * ts;


		Frostic::RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 1 });
		Frostic::RenderCommand::Clear();

		m_Camera.SetPostion(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Frostic::Renderer::BeginScene(m_Camera);

		Frostic::Renderer::Submit(m_SquareShader, m_SquareVA);
		Frostic::Renderer::Submit(m_Shader, m_VertexArray);

		Frostic::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		
	}

	void OnEvent(Frostic::Event& event) override
	{
		
	}
private:
	std::shared_ptr<Frostic::Shader> m_Shader;
	std::shared_ptr<Frostic::VertexArray> m_VertexArray;

	std::shared_ptr<Frostic::Shader> m_SquareShader;
	std::shared_ptr<Frostic::VertexArray> m_SquareVA;

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