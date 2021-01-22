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
	
	// TODO: Remove!
	Frostic::Ref<Frostic::VertexArray> m_SquareVA;
	Frostic::Ref<Frostic::Shader> m_FlatColorShader;

	glm::vec4 m_SquareColor = { 0.2, 0.3f, 0.8f, 0.8f };
};