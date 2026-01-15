#include <windows.h>
#include "tools.h"
#include "windowManager.h"


int main(int argc, char** argv)
{
#ifndef _DEBUG
	FreeConsole();
#endif // !DEBUG

	sfVector2i defaultVideoMode = { 1920,1080 };
	// Struct to config the window
	Window* mainWindow = windowSetup("CSFML la Base!!!", defaultVideoMode);
	// Init time
	initTools();
	// Init
	windowInit(mainWindow);

	// Loop
	while (!IsDone(mainWindow))
	{
		windowUpdate(mainWindow);

		// Display
		BeginDraw(mainWindow);
		windowDraw(mainWindow);
		EndDraw(mainWindow);
	}
	return 0;
}