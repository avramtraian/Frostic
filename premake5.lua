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
IncludeDir["GLFW"] = "%{wks.location}/Frostic/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Frostic/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Frostic/vendor/Imgui"
IncludeDir["glm"] = "%{wks.location}/Frostic/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Frostic/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Frostic/vendor/entt/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Frostic/vendor/ImGuizmo"
IncludeDir["yaml_cpp"] = "%{wks.location}/Frostic/vendor/yaml-cpp/include"

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

	pchheader "fepch.h"
	pchsource "Frostic/src/fepch.cpp"

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
			"FE_PLATFORM_WINDOWS",
			"FE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "FE_DEBUG"
		defines "FE_ENABLE_ASSERTS"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FE_RELEASE"
		defines "FE_ENABLE_ASSERTS"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "FE_DIST"
		runtime "Release"
		optimize "on"

	filter "files:Frostic/vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

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
		"Frosted/src",
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
			"FE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "FE_DEBUG"
		defines "FE_ENABLE_ASSERTS"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "FE_RELEASE"
		defines "FE_ENABLE_ASSERTS"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "FE_DIST"
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
		"%{prj.name}/src",
		"Frostic/vendor/spdlog/include",
		"Frostic/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
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
			"FE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "FE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FE_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "FE_DIST"
		runtime "Release"
		optimize "on"