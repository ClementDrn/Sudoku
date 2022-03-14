--- Solution -------------------------------
workspace "Sudoku"
   
	configurations { 
	   "Debug", 
	   "Release"
	}

	platforms {
		"x86",
		"x64"
	}

	startproject "Sudoku"

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x86_64"


--- Variables ------------------------------
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includedir = {}
includedir["SFML"] = "Sudoku/vendor/SFML/include"
includedir["SEL"] = "Sudoku/vendor/SEL/include"
includedir["ImGui"] = "Sudoku/vendor/imgui"
includedir["ImGui_SFML"] = "Sudoku/vendor/imgui-sfml"


--- Dependencies ---------------------------
group "Dependencies"
	include "Sudoku/vendor/SFML"
	include "Sudoku/vendor/imgui"
group ""


--- Sudoku --------------------------------
project "Sudoku"

	location "Sudoku"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("obj/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{includedir.ImGui_SFML}/imgui-SFML.h",
		"%{includedir.ImGui_SFML}/imgui-SFML.cpp"
	}

	includedirs {
		"%{includedir.SFML}",
		"%{includedir.SEL}",
		"%{includedir.ImGui}",
	}

	defines {
		"SFML_STATIC",
		"IMGUI_SFML"
	}

	filter "system:windows"
		systemversion "latest"
		libdirs {
			"%{prj.name}/vendor/SFML/extlibs/libs-msvc-universal/%{cfg.platform}/"
		}
	
	filter {}
	links {
		"opengl32.lib",
		"freetype.lib",
		"winmm.lib",
		"gdi32.lib",
		"ws2_32.lib",
		"SFML",
		"ImGui"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
