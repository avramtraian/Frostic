#include "Sandbox2D.h"
#include "Imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1600.0f / 900.0f)
{
}

void Sandbox2D::OnAttach()
{
	FR_PROFILE_FUNCTION();

	m_Texture = Frostic::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	FR_PROFILE_FUNCTION();
	
}

void Sandbox2D::OnUpdate(Frostic::Timestep ts)
{
	FR_PROFILE_FUNCTION();

	m_FPS = (uint32_t)(1.0f / ts);

	// Updating
	m_CameraController.OnUpdate(ts);

	// Rendering
	Frostic::Renderer2D::ResetStats();
	{
		FR_PROFILE_SCOPE("Renderer preparation");
		Frostic::RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 1 });
		Frostic::RenderCommand::Clear();
	}
	{
		FR_PROFILE_SCOPE("Renderer Draw");
		Frostic::Renderer2D::BeginScene(m_CameraController.GetCamera());
		float size = 6.0f;
		for (float y = -size; y < size; y += 0.1f)
		{
			for (float x = -size; x < size; x += 0.1f)
			{
				glm::vec3 color = { (x + 5.0f) / 10.0f, 0.3f, (y + 5.0f) / 10.0f };
				Frostic::Renderer2D::DrawQuad({ x, y }, { 0.085f, 0.085f }, { color.x, color.y, color.z, 1.0f });
			}
		}
		Frostic::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	FR_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Frostic::Renderer2D::GetStats();
	ImGui::Text("FPS: %d", m_FPS);
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Frostic::Event& e)
{
	m_CameraController.OnEvent(e);
}