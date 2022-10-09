project "ClientFW"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "stdafx.h"
	pchsource "src/stdafx.cpp"

	
	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"vendor/assimp/include",
		"vendor/assimp/build/include",
	}

	links
	{
		"vendor/assimp/build/bin/Debug/assimp-vc143-mtd.dll",
		"vendor/assimp/build/lib/Debug/assimp-vc143-mtd.lib",
	}

	debugenvs
	{
		"vendor/assimp/build/bin/Debug/assimp-vc143-mtd.dll",
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "ClientFW_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ClientFW_RELEASE"
		optimize "On"