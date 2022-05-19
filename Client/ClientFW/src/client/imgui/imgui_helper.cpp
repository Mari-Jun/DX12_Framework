#include "stdafx.h"
#include "client/imgui/imgui_helper.h"

namespace client_fw
{
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

}