#include "Sandbox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D")
{
	Frostic::SceneManagerRuntime::Initialize(FE_BIND_EVENT_FN(Sandbox2D::OnLoadScene), FE_BIND_EVENT_FN(Sandbox2D::OnUnloadScene));
}

void Sandbox2D::OnAttach()
{
	FE_PROFILE_FUNCTION();

	Frostic::SceneManagerRuntime::LoadScene(0);
}

void Sandbox2D::OnDetach()
{
	FE_PROFILE_FUNCTION();
	
}

void Sandbox2D::OnUpdate(Frostic::Timestep ts)
{
	FE_PROFILE_FUNCTION();

	Frostic::RenderCommand::SetClearColor({ 0.075f, 0.075f, 0.075f, 1.0f });
	Frostic::RenderCommand::Clear();

	m_ActiveScene->OnUpdateRuntime(ts);
}

void Sandbox2D::OnEvent(Frostic::Event& e)
{
	
}

void Sandbox2D::OnLoadScene(Frostic::Ref<Frostic::Scene>& scene)
{
	m_ActiveScene = scene;
	m_ActiveScene->OnViewportResize(2560, 1440);
}

void Sandbox2D::OnUnloadScene(int i)
{

}