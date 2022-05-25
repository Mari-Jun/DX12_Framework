#include "stdafx.h"
#include "client/imgui/imgui_helper.h"

namespace client_fw
{
	ImFont* ImGuiHelper::s_jua_14 = nullptr;
	ImFont* ImGuiHelper::s_jua_20 = nullptr;
	ImFont* ImGuiHelper::s_jua_24 = nullptr;
	ImFont* ImGuiHelper::s_jua_32 = nullptr;

	ImVec4 ImGuiHelper::GetImGuiWindowSize()
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		vMin.x += ImGui::GetWindowPos().x - ImGui::GetStyle().WindowPadding.x;
		vMin.y += ImGui::GetWindowPos().y - ImGui::GetStyle().WindowPadding.y;
		vMax.x += ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x;
		vMax.y += ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y;

		ImVec4 result(vMin.x, vMin.y, vMax.x - vMin.x, vMax.y - vMin.y);

		return result;
	}

	ImVec4 ImGuiHelper::GetImGuiWindowBoxSize()
	{
		ImVec4 result = GetImGuiWindowSize();
		result.x += ImGui::GetStyle().WindowPadding.x + ImGui::GetStyle().FramePadding.x;
		result.y += ImGui::GetStyle().WindowPadding.y + ImGui::GetStyle().FramePadding.y;
		result.z -= ImGui::GetStyle().WindowPadding.x * 2;
		result.w -= ImGui::GetStyle().WindowPadding.y * 2;
		return result;
	}

	void ImGuiHelper::AlignFormWidth(float width, float alignment)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		float avail = ImGui::GetContentRegionAvail().x;
		float offset = (avail - width) * alignment;
		if (offset > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + offset);
	}

	void ImGuiHelper::GenerateJuaFonts()
	{
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		s_jua_14 = io.Fonts->AddFontFromFileTTF("../Contents/Fonts/BMJUA_ttf.ttf", 14.f, nullptr, io.Fonts->GetGlyphRangesKorean());
		s_jua_20 = io.Fonts->AddFontFromFileTTF("../Contents/Fonts/BMJUA_ttf.ttf", 20.f, nullptr, io.Fonts->GetGlyphRangesKorean());
		s_jua_24 = io.Fonts->AddFontFromFileTTF("../Contents/Fonts/BMJUA_ttf.ttf", 24.f, nullptr, io.Fonts->GetGlyphRangesKorean());
		s_jua_32 = io.Fonts->AddFontFromFileTTF("../Contents/Fonts/BMJUA_ttf.ttf", 32.f, nullptr, io.Fonts->GetGlyphRangesKorean());
	}
}