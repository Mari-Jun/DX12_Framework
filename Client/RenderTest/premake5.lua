project "RenderTest"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	includedirs
	{
		"%{wks.location}/ClientFW/src",
		"%{wks.location}/ClientFW/vendor/spdlog/include",
		"%{wks.location}/RenderTest/src",
	}
	
	links
	{
		"ClientFW"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "RenderTest_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RenderTest_RELEASE"
		optimize "On"