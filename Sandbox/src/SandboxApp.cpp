#include <Frostic.h>
#include <Frostic/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox : public Frostic::Application
{
public:
	Sandbox()
	{
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