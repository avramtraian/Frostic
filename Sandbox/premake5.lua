project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Frostic/vendor/spdlog/include",
		"Frostic/vendor",
		"%{IncludeDir.glm}",
		"Frostic/src"
	}

	links
	{
		"Frostic"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "FR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "FR_DIST"
		runtime "Release"
		optimize "on"