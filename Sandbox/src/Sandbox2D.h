#pragma once
#include <Frostic.h>

class Sandbox2D : public Frostic::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Frostic::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Frostic::Event& e) override;
private:
	Frostic::OrthographicCameraController m_CameraController;

	glm::vec4 m_Color = { 0.2f, 0.3f, 0.8f, 1.0f };
	Frostic::Ref<Frostic::Texture2D> m_Texture;
};