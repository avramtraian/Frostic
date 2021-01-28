#include <Frostic.h>
#include <Frostic/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Frostic {

	class Frosted : public Application
	{
	public:
		Frosted()
			: Application("Frosted")
		{
			//PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}
		~Frosted()
		{

		}
	};

	Application* CreateApplication()
	{
		return new Frosted();
	}

}