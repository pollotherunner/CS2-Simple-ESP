#pragma once
#include <Windows.h>
#include "../utils/sdk.hpp"
#include "../memory/memory.hpp"

class Overlay
{
public:
	static void Drawing();
	static void Render();

	static inline bool Enabled;
};