#include <WS2812FX.h>

#include "IRLremote.h"
CHashIR IRLremote;
uint32_t IRdata;

#define LED_COUNT 64

#define LED_PIN 12
#define IR_PIN 2

int maxBright = 255; // 0-255
int currentBright = maxBright / 2;
int brightStep = 25; // step for changing brightness
bool brightChanged = false; // was bright changed in current tick

bool isLedsOn = false; //switcher for light

int prevMode = FX_MODE_CHASE_RANDOM;
int currentMode = FX_MODE_CHASE_RANDOM;
bool modeChanged = false; // was mode changed in current tick

uint32_t colors[] = {
  PINK, CYAN, MAGENTA, PURPLE, ORANGE,
  (uint32_t)0xFAEBD7, // AntiqueWhite
  (uint32_t)0x7FFFD4  // Aquamarine
};
int colorsSize =  sizeof(colors);
int _currentColorIndex = 0;
uint32_t colorBase = colors[_currentColorIndex]; // color used for setColor function
bool colorBaseChanged = false; // was base color changed in current tick

bool ir_flag = false; // was data received in current tick

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

#define BUTT_UP     0xF39EEBAD
#define BUTT_DOWN   0xC089F6AD
#define BUTT_LEFT   0xE25410AD
#define BUTT_RIGHT  0x14CE54AD
#define BUTT_OK     0x297C76AD
#define BUTT_1      0x4E5BA3AD
#define BUTT_2      0xE51CA6AD
#define BUTT_3      0xE207E1AD
#define BUTT_4      0x517068AD
#define BUTT_5      0x1B92DDAD
#define BUTT_6      0xAC2A56AD
#define BUTT_7      0x5484B6AD
#define BUTT_8      0xD22353AD
#define BUTT_9      0xDF3F4BAD
#define BUTT_0      0xF08A26AD
#define BUTT_STAR   0x68E456AD
#define BUTT_HASH   0x151CD6AD

void setup() {
    Serial.begin(9600);
    IRLremote.begin(IR_PIN);

    ws2812fx.init();

    ws2812fx.setBrightness(currentBright);

    // 10=very fast, 5000=very slow
    ws2812fx.setSpeed(2000);

    ws2812fx.setColor(colorBase);
    ws2812fx.setMode(FX_MODE_LARSON_SCANNER);

    ws2812fx.start();
}

void loop() {
    if (IRLremote.available())  {
      auto data = IRLremote.read();
      IRdata = data.command;
      ir_flag = true;

      /* Remote debug */
      /* Serial.print(F("Address: 0x")); */
      /* Serial.println(data.address, HEX); */
      /* Serial.print(F("Command: 0x")); */
      /* Serial.println(data.command, HEX); */
      /* Serial.println(); */
    }
    if (ir_flag) {
      switch (IRdata) {
        case BUTT_STAR:
          break;
        case BUTT_1: setMode(FX_MODE_STATIC);
          break;
        case BUTT_2: setMode(FX_MODE_THEATER_CHASE);
          break;
        case BUTT_3: setMode(FX_MODE_STROBE_RAINBOW);
          break;
        case BUTT_4: setMode(FX_MODE_MERRY_CHRISTMAS);
          break;
        case BUTT_5: setMode(FX_MODE_SCAN);
          break;
        case BUTT_6: setMode(FX_MODE_COLOR_WIPE);
          break;
        case BUTT_7: setMode(FX_MODE_LARSON_SCANNER);
          break;
        case BUTT_8: setMode(FX_MODE_CHASE_RAINBOW);
          break;
        case BUTT_9: setMode(FX_MODE_SPARKLE);
          break;
        case BUTT_0: setMode(FX_MODE_BREATH);
          break;
        case BUTT_LEFT: setPrevColor();
          break;
        case BUTT_RIGHT: setNextColor();
          break;
        case BUTT_UP: increaseBright();
          break;
        case BUTT_DOWN: decreaseBright();
          break;
        default:
          break;
      }

      if (modeChanged) {
        ws2812fx.setMode(currentMode);
        modeChanged = false;
      }

      if (colorBaseChanged) {
        ws2812fx.setColor(colorBase);
        colorBaseChanged = false;
      }

      if (brightChanged) {
        ws2812fx.setBrightness(currentBright);
        brightChanged = false;
      }

      ir_flag = false;
    }
    if (!IRLremote.receiving())
      ws2812fx.service();
}

void setNextColor() {
  if (_currentColorIndex == colorsSize - 1)
    _currentColorIndex = 0;
  else
    _currentColorIndex++;

  colorBaseChanged = true;
  colorBase = colors[_currentColorIndex];
}

void setPrevColor() {
  if (_currentColorIndex == 0)
    _currentColorIndex = colorsSize - 1;
  else
    _currentColorIndex--;

  colorBaseChanged = true;
  colorBase = colors[_currentColorIndex];
}

void setMode(int newMode) {
  if (newMode == currentMode){return;}

  modeChanged = true;
  prevMode = currentMode;
  currentMode = newMode;
}

void increaseBright() {
  if (currentBright + brightStep > maxBright)
    currentBright = maxBright;
  else
    currentBright += brightStep;

  brightChanged = true;
}

void decreaseBright() {
  if (currentBright - brightStep < 0)
    currentBright = 0;
  else
    currentBright -= brightStep;

  brightChanged = true;
}
