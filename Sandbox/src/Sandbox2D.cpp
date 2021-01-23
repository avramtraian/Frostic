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

	// Updating
	m_CameraController.OnUpdate(ts);

	// Rendering
	{
		FR_PROFILE_SCOPE("Renderer preparation");
		Frostic::RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 1 });
		Frostic::RenderCommand::Clear();
	}
	{
		FR_PROFILE_SCOPE("Renderer Draw");
		Frostic::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Frostic::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture);
		Frostic::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Frostic::Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, 45.0f, { 0.5f, 0.75f }, m_Color);
		Frostic::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	FR_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_Color));
	ImGui::End();
}

void Sandbox2D::OnEvent(Frostic::Event& e)
{
	m_CameraController.OnEvent(e);
}