#include <../features/esp.hpp>

void esp::start() {


    const auto draw_list = ImGui::GetBackgroundDrawList();
    uintptr_t entityList = memory::read<uintptr_t>(memory::base_addr + g_offsets.entity_list);
    uintptr_t listEntry = memory::read<uintptr_t>(entityList + 0x10);
    SDK::viewmatrix = memory::read<SDK::view_matrix_t>(memory::base_addr + g_offsets.view_matrix);


    for (int i = 0; i < 64; i++)
    {
        if (!listEntry) continue;

        uintptr_t currentController = memory::read<uintptr_t>(listEntry + i * 0x78);

        if (!currentController) continue;

        int pawnHandle = memory::read<int>(currentController + g_offsets.m_hPlayerPawn);

        if (!pawnHandle) continue;


        uintptr_t listEntry2 = memory::read<uintptr_t>(entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

        uintptr_t currentPawn = memory::read<uintptr_t>(listEntry2 + 0x78 * (pawnHandle & 0x1FF));
        D3DXVECTOR2 entity_pos;
       // ImVec2 wts_entity_pos = ImVec2(entity_pos.x, entity_pos.y);
        if (g_settings.ignore_dead)
        {
            if (SDK::get_health(currentPawn) <= 0)continue;
        }
        if (!SDK::WorldToScreen(SDK::get_pos(currentPawn), entity_pos))continue;

        if (g_settings.lines) {
            draw_list->AddLine(ImVec2(g_variables.r.right/2, g_variables.r.bottom), ImVec2(entity_pos.x, entity_pos.y), ImColor(g_settings.esp_color[0], g_settings.esp_color[1], g_settings.esp_color[2]));
        }
        if (g_settings.names) {
            draw_list->AddText(ImVec2(entity_pos.x, entity_pos.y), ImColor(g_settings.esp_color[0], g_settings.esp_color[1], g_settings.esp_color[2]), SDK::get_name(currentController).c_str());
        }
       

    }
}