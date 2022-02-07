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


--- Dependencies ---------------------------
group "Dependencies"
	include "Sudoku/vendor/SFML"
group ""


--- Sudoku --------------------------------
project "Sudoku"

	location "Sudoku"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c"
	}

	includedirs {
		"%{includedir.SFML}"
	}

	links {
		"SFML"
	}

	filter "system:windows"
		systemversion "latest"
	
	filter "configurations:Debug"
		defines "PINE_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "PINE_RELEASE"
		runtime "Release"
		optimize "on"
