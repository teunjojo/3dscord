#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static MiiSelectorConf msConf;
static MiiSelectorReturn msRet;
static uint8_t miiStudioData[0x2F];

uint8_t next = 0;
size_t pos = 1;

void encodeMiiPart(uint8_t partValue) {
  uint8_t encoded = (7 + (partValue ^ next)) % 256;
  next = encoded;
  miiStudioData[pos] = encoded;
  pos++;
}

uint8_t *encodeStudio(MiiData *mii) {

  memset(miiStudioData, 0, sizeof(miiStudioData));

  encodeMiiPart(mii->beard_details.color == 0 ? 8 : mii->beard_details.color);
  encodeMiiPart(mii->beard_details.style);
  encodeMiiPart(mii->width);
  encodeMiiPart(mii->eye_details.yscale);
  encodeMiiPart(mii->eye_details.color + 8);
  encodeMiiPart(mii->eye_details.rotation);
  encodeMiiPart(mii->eye_details.scale);
  encodeMiiPart(mii->eye_details.style);
  encodeMiiPart(mii->eye_details.xspacing);
  encodeMiiPart(mii->eye_details.yposition);
  encodeMiiPart(mii->eyebrow_details.yscale);
  encodeMiiPart(mii->eyebrow_details.color == 0 ? 8
                                                : mii->eyebrow_details.color);
  encodeMiiPart(mii->eyebrow_details.rotation);
  encodeMiiPart(mii->eyebrow_details.scale);
  encodeMiiPart(mii->eyebrow_details.style);
  encodeMiiPart(mii->eyebrow_details.xspacing);
  encodeMiiPart(mii->eyebrow_details.yposition);
  encodeMiiPart(mii->face_style.skinColor);
  encodeMiiPart(mii->face_details.makeup);
  encodeMiiPart(mii->face_style.shape);
  encodeMiiPart(mii->face_details.wrinkles);
  encodeMiiPart(mii->mii_details.shirt_color);
  encodeMiiPart(mii->mii_details.sex);

  if (mii->glasses_details.color == 0) {
    encodeMiiPart(8);
  } else if (mii->glasses_details.color < 6) {
    encodeMiiPart(mii->glasses_details.color + 13);
  } else {
    encodeMiiPart(0);
  }

  encodeMiiPart(mii->glasses_details.scale);
  encodeMiiPart(mii->glasses_details.style);
  encodeMiiPart(mii->glasses_details.ypos);
  encodeMiiPart(mii->hair_details.color == 0 ? 8 : mii->hair_details.color);
  encodeMiiPart(mii->hair_details.flip ? 1 : 0); // 30
  encodeMiiPart(mii->hair_style);
  encodeMiiPart(mii->height);
  encodeMiiPart(mii->mole_details.scale);
  encodeMiiPart(mii->mole_details.enable ? 1 : 0);
  encodeMiiPart(mii->mole_details.xpos);
  encodeMiiPart(mii->mole_details.ypos);
  encodeMiiPart(mii->mouth_details.yscale);

  if (mii->mouth_details.color < 4) {
    encodeMiiPart(mii->mouth_details.color + 19);
  } else {
    encodeMiiPart(0);
  }

  encodeMiiPart(mii->mouth_details.scale);
  encodeMiiPart(mii->mouth_details.style); // 30
  encodeMiiPart(mii->mustache_details.mouth_yposition);
  encodeMiiPart(mii->beard_details.scale);
  encodeMiiPart(mii->mustache_details.mustach_style);
  encodeMiiPart(mii->beard_details.ypos);
  encodeMiiPart(mii->nose_details.scale);
  encodeMiiPart(mii->nose_details.style);
  encodeMiiPart(mii->nose_details.yposition);

  return miiStudioData;
}

void bufferToHex(const uint8_t *buffer, size_t length, char *hexString) {
  const char hexDigits[] = "0123456789abcdef";
  for (size_t i = 0; i < length; ++i) {
    hexString[i * 2] = hexDigits[(buffer[i] >> 4) & 0x0F];
    hexString[i * 2 + 1] = hexDigits[buffer[i] & 0x0F];
  }
  hexString[length * 2] = '\0'; // Null-terminate the string
}

void saveMii() {

  if (!miiSelectorChecksumIsValid(&msRet)) {
    printf("Return checksum invalid.\n");
    return;
  }

  if (msRet.no_mii_selected) {
    printf("No Mii was selected.\n");
    return;
  }

  uint8_t *encodedData = encodeStudio(&msRet.mii);

  char hexString[0x2F * 2 +
                 1]; // Each byte as two hex characters plus null terminator
  bufferToHex(encodedData, 0x2F, hexString);

  printf("Encoded data: %s\n", hexString);

  // Write encoded MiiData String to file
  FILE *file = fopen("miiData", "w");
  if (!file) {
    printf("Failed to open file for writing.\n");
    return;
  }
  printf("Writing encoded MiiData to file...");
  fprintf(file, "%s", hexString);
  fclose(file);
  printf("done.\n");
}

void selectMii() {
  miiSelectorInit(&msConf);
  miiSelectorLaunch(&msConf, &msRet);
  saveMii();
}

void promptUser() {
  printf("Press A to bring up Mii selector with default settings.\n");
  cfguInit();
  u16 username[0x1C];
  CFGU_GetConfigInfoBlk2(0x1C, 0x000A0000, username);
  char utf8_username[11];
  ssize_t len = utf16_to_utf8((uint8_t *)utf8_username, username,
                              sizeof(utf8_username) - 1);
  utf8_username[len] = '\0';
  printf("Username: %s\n", utf8_username);
  printf("Press START to exit.\n");
}