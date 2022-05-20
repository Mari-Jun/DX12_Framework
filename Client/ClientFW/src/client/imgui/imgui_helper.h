#pragma once
#include "client/imgui/imgui.h"

namespace client_fw
{
	class ImGuiHelper final
	{
	public:
		static ImVec4 GetImGuiWindowSize();
		static ImVec4 GetImGuiWindowBoxSize();

		static void AlignFormWitdh(float width, float alignment = 0.5f);
	};
}

