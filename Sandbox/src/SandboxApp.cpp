#include <Frostic.h>

class Sandbox : public Frostic::Application
{
public:
	Sandbox()
	{
	}
	~Sandbox()
	{
	}
};

Frostic::Application* Frostic::CreateApplication()
{
	return new Sandbox();
}