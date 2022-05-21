#pragma once
#include "client/imgui/imgui.h"

namespace client_fw
{
	class ImGuiHelper final
	{
	public:
		static ImVec4 GetImGuiWindowSize();
		static ImVec4 GetImGuiWindowBoxSize();

		static void AlignFormWidth(float width, float alignment = 0.5f);

		static void GenerateJuaFonts();
		static ImFont* s_jua_14;
		static ImFont* s_jua_20;
		static ImFont* s_jua_24;
		static ImFont* s_jua_32;
	};
}

