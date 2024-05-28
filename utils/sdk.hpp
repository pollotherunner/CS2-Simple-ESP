#pragma once
#include <D3dx9math.h>
#include "../memory/memory.hpp"
class offsets {
public:
    //offsets
    const int entity_list = 0x19A3328;
    const int local_player = 0x180DB18;
    const int view_angles = 0x1A12498;
    const int view_matrix = 0x1A052D0;

   // namespace C_BaseEntity {
    const int m_iHealth = 0x324;
    const int m_vOldOrigin = 0x1274;

   // namespace CCSPlayerController {
    const int m_hPlayerPawn = 0x7DC;

	// namespace CBasePlayerController {
	const int m_iszPlayerName = 0x630;

};
inline offsets g_offsets;

class c_globals {
public:
	HWND hWindow;
	HWND monitor;
	RECT r;
	RECT rm;
};
inline c_globals g_variables;

class c_settings {
public:
	bool lines;
	bool names;
	bool ignore_dead;
	float esp_color[3] = {255,255,255};
};
inline c_settings g_settings;



namespace SDK {

	inline std::string get_name(uintptr_t controler) {
		return memory::ReadString(controler + g_offsets.m_iszPlayerName);
	}

    inline int get_health(uintptr_t pawn) {
        return memory::read<int>(pawn + g_offsets.m_iHealth);
    }

    inline D3DXVECTOR3 get_pos(uintptr_t pawn) {
        return memory::read<D3DXVECTOR3>(pawn + g_offsets.m_vOldOrigin);
    }

	struct view_matrix_t {
		float* operator[ ](int index) {
			return matrix[index];
		}

		float matrix[4][4];
	};

	inline view_matrix_t viewmatrix;

	inline bool WorldToScreen(const D3DXVECTOR3& Pos, D3DXVECTOR2& ToPos)
	{
		float View = 0.f;
		float SightX = (g_variables.r.right - g_variables.r.left)/2, SightY = (g_variables.r.bottom - g_variables.r.top)/2;

		View = viewmatrix[3][0] * Pos.x + viewmatrix[3][1] * Pos.y + viewmatrix[3][2] * Pos.z + viewmatrix[3][3];

		if (View <= 0.01)
			return false;

		ToPos.x = SightX + (viewmatrix[0][0] * Pos.x + viewmatrix[0][1] * Pos.y + viewmatrix[0][2] * Pos.z + viewmatrix[0][3]) / View * SightX;
		ToPos.y = SightY - (viewmatrix[1][0] * Pos.x + viewmatrix[1][1] * Pos.y + viewmatrix[1][2] * Pos.z + viewmatrix[1][3]) / View * SightY;

		return true;
	}
}

