#include <Frostic.h>

class ExampleLayer : public Frostic::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		
	}

	void OnEvent(Frostic::Event& event) override
	{
		
	}

};

class Sandbox : public Frostic::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Frostic::ImGuiLayer());
	}
	~Sandbox()
	{

	}
};

Frostic::Application* Frostic::CreateApplication()
{
	return new Sandbox();
}