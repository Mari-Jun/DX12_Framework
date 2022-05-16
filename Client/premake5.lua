workspace "DX12Framework"
	architecture "x64"
	startproject "RenderTest"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

IncludeDir = {}

group "Dependencies"
group ""

include "ClientFW"
include "ClientTest"
include "EventTest"
include "RenderTest"
