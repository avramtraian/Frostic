workspace "Frostic"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Frostic/vendor/GLFW/include"
IncludeDir["Glad"] = "Frostic/vendor/Glad/include"
IncludeDir["ImGui"] = "Frostic/vendor/Imgui"
IncludeDir["glm"] = "Frostic/vendor/glm"

include "Frostic/vendor/GLFW"
include "Frostic/vendor/Glad"
include "Frostic/vendor/Imgui"

project "Frostic"
	location "Frostic"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "frpch.h"
	pchsource "Frostic/src/frpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"FR_PLATFORM_WINDOWS",
			"FR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "FR_DEBUG"
		defines "FR_ENABLE_ASSERTS"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FR_RELEASE"
		defines "FR_ENABLE_ASSERTS"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "FR_DIST"
		runtime "Release"
		optimize "on"

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