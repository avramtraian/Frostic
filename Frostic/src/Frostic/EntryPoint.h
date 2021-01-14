#pragma once

#ifdef FR_PLATFORM_WINDOWS

extern Frostic::Application* Frostic::CreateApplication();

int main(int argc, char** argv)
{
	Frostic::Log::Init();
	FR_CORE_WARN("Initialize Log!");
	FR_INFO("Hello!");

	auto app = Frostic::CreateApplication();
	app->Run();
	delete app;
}

#endif