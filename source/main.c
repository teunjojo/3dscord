#include "./selectMii.h"'
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  gfxInitDefault();
  consoleInit(GFX_TOP, NULL);

  cfguInit();
  u16 username[0x1C];
  CFGU_GetConfigInfoBlk2(0x1C, 0x000A0000, username);
  char utf8_username[11];
  ssize_t len = utf16_to_utf8((uint8_t *)utf8_username, username,
                              sizeof(utf8_username) - 1);
  utf8_username[len] = '\0';
  printf("Username: %s\n", utf8_username);

  promptUser();

  // Main loop
  while (aptMainLoop()) {
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
