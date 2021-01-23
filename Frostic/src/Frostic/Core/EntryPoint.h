#pragma once

#ifdef FR_PLATFORM_WINDOWS

extern Frostic::Application* Frostic::CreateApplication();

int main(int argc, char** argv)
{
	Frostic::Log::Init();

	FR_PROFILE_BEGIN_SESSION("Startup", "FrosticProfile-Startup.json");
	auto app = Frostic::CreateApplication();
	FR_PROFILE_END_SESSION();

	FR_PROFILE_BEGIN_SESSION("Runtime", "FrosticProfile-Runtime.json");
	app->Run();
	FR_PROFILE_END_SESSION();

	FR_PROFILE_BEGIN_SESSION("Shutdown", "FrosticProfile-Shutdown.json");
	delete app;
	FR_PROFILE_END_SESSION();
}

#endif