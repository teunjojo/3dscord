#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include "./selectMii.h"'

int main(int argc, char* argv[])
{
	gfxInitDefault();
	consoleInit(GFX_TOP, NULL);

	promptUser();

	// Main loop
	while (aptMainLoop())
	{
		gspWaitForVBlank();
		gfxSwapBuffers();
		hidScanInput();

		// Your code goes here
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		
		if (kDown & KEY_A)
			selectMii();
	}

	gfxExit();
	return 0;
}
