#include "Overlay.h"
#include <imgui.h>

void Overlay::Drawing()
{
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui::SetNextWindowSize(ImVec2(600, 400));

	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoDecoration);
	{
	

		ImGui::Checkbox("ESP Line", &g_settings.lines);		
		ImGui::Checkbox("ESP Name", &g_settings.names);
		ImGui::Checkbox("Ignore Dead", &g_settings.ignore_dead);
		ImGui::ColorPicker3("ESP Color", g_settings.esp_color);
		
	}
	ImGui::End();
	
}