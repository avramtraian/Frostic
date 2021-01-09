#pragma once

#include "Core.h"

namespace Frostic {

	class FROSTIC_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
	
	// To be defined in CLIENT
	Application* CreateApplication();

}