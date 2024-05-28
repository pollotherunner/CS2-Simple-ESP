#include "Menu/Overlay.h"

inline void log(std::string text) {
	std::cout << text << '\n';
}

int main()
{
	memory::open_proc("cs2.exe");
	g_variables.hWindow = FindWindowA(nullptr, "Counter-Strike 2");
	GetWindowRect(g_variables.hWindow, &g_variables.r);
	g_variables.monitor = GetDesktopWindow();
	GetWindowRect(g_variables.monitor, &g_variables.rm);
	log("simple ESP by pollotherunner [PRESS INSERT TO OPEN]");
	log("cs2 rect: " + std::to_string(g_variables.r.right - g_variables.r.left) + "/" + std::to_string(g_variables.r.bottom- g_variables.r.top));
	
	Overlay::Render();

	return 0;
}