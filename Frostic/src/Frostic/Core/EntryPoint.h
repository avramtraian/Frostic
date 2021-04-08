#pragma once

#ifdef FE_PLATFORM_WINDOWS

extern Frostic::Application* Frostic::CreateApplication();

int main(int argc, char** argv)
{
	Frostic::Log::Init();

	FE_PROFILE_BEGIN_SESSION("Startup", "FrosticProfile-Startup.json");
	auto app = Frostic::CreateApplication();
	FE_PROFILE_END_SESSION();

	FE_PROFILE_BEGIN_SESSION("Runtime", "FrosticProfile-Runtime.json");
	app->Run();
	FE_PROFILE_END_SESSION();

	FE_PROFILE_BEGIN_SESSION("Shutdown", "FrosticProfile-Shutdown.json");
	delete app;
	FE_PROFILE_END_SESSION();
}

#endif