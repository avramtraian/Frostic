#pragma once

#ifdef FR_PLATFORM_WINDOWS

extern Frostic::Application* Frostic::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Frostic::CreateApplication();
	app->Run();
	delete app;
}

#endif