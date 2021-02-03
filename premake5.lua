workspace "Frostic"
	architecture "x64"
	startproject "Frosted"

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
IncludeDir["stb_image"] = "Frostic/vendor/stb_image"
IncludeDir["entt"] = "Frostic/vendor/entt/include"
IncludeDir["ImGuizmo"] = "Frostic/vendor/ImGuizmo"
IncludeDir["yaml_cpp"] = "Frostic/vendor/yaml-cpp/include"

group "Dependencies"
	include "Frostic/vendor/GLFW"
	include "Frostic/vendor/Glad"
	include "Frostic/vendor/Imgui"
	include "Frostic/vendor/yaml-cpp"
group ""

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
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
		"%{prj.name}/vendor/entt/**.hpp"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.yaml_cpp}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
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

	filter "files:Frostic/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

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

project "Frosted"
	location "Frosted"
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
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
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