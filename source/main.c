#include "./selectMii.h"
#include <citro2d.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 240

void renderScene(C3D_RenderTarget *screen, C2D_Text *dynTimeText) {
  u32 white = C2D_Color32(0xff, 0xff, 0xff, 0xFF);
  u32 gray = C2D_Color32(0xe5, 0xe5, 0xe5, 0xFF);
  C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
  C2D_TargetClear(screen, white);
  C2D_SceneBegin(screen);

  // Status bar
  C2D_DrawRectangle(0, 0, 0, SCREEN_WIDTH, 24, gray, gray, gray, gray);

  // Draw the time
  C2D_DrawText(dynTimeText, C2D_AlignLeft, 360, 4, 1, .5, .5);

  C3D_FrameEnd(0);
}

int main(int argc, char *argv[]) {
  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
  consoleInit(GFX_BOTTOM, NULL);

  // Create screens
	C3D_RenderTarget* screen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

  cfguInit();
  u16 username[0x1C];
  CFGU_GetConfigInfoBlk2(0x1C, 0x000A0000, username);
  char utf8_username[11];
  ssize_t len = utf16_to_utf8((uint8_t *)utf8_username, username,
                              sizeof(utf8_username) - 1);
  utf8_username[len] = '\0';

  C2D_TextBuf dynTimeBuf = C2D_TextBufNew(4096);
C2D_Text dynTimeText;

  // Main loop
  while (aptMainLoop()) {
    hidScanInput();

    // Your code goes here
    u32 kDown = hidKeysDown();
    if (kDown & KEY_START)
      break; // break in order to return to hbmenu

    if (kDown & KEY_A)
      selectMii();

      // Get the current time as a string
  time_t unixTime = time(NULL);
  struct tm* timeStruct = gmtime((const time_t *)&unixTime);
  char timeString[9];
  strftime(timeString, sizeof(timeString), "%H:%M", timeStruct);

    // Parse the time into the text buffer
  C2D_TextBufClear(dynTimeBuf);
  C2D_TextParse(&dynTimeText, dynTimeBuf, timeString);
  C2D_TextOptimize(&dynTimeText);

    renderScene(screen, &dynTimeText);
    
  }

  C2D_Fini();
	C3D_Fini();
  gfxExit();
  return 0;
}
