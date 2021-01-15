#include <Frostic.h>

#include "Imgui/imgui.h"

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

	void OnImGuiRender()
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World!");
		ImGui::End();
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
	}
	~Sandbox()
	{

	}
};

Frostic::Application* Frostic::CreateApplication()
{
	return new Sandbox();
}