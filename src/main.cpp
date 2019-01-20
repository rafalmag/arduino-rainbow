
/*
 * References:
 * 
 * http://fastled.io/docs/3.1/group___colorutils.html
 * https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list
 *
 */

#include "FastLED.h" // FastLED library.

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// Fixed definitions cannot change on the fly.
#define DATA_PIN 6
#define LED_TYPE NEOPIXEL
#define NUM_LEDS 60

// Initialize LED array.
struct CRGB leds[NUM_LEDS];

uint8_t startpos = 0;
int endpos = NUM_LEDS - 1;

// encoder
#define encoder0PinA 9
#define encoder0PinB 10
#define encoder0Button 11

int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

// other Global variables can be changed on the fly.
// Overall brightness definition. It can be changed on the fly.
uint8_t max_bright = 128;
unsigned long lastChangeMs = millis();

// declared here as relies on leds, NUM_LEDS, lastChangeMs
#include <palettes.h>
#include <palettesFunctions.h>

void setup()
{
  Serial.begin(9600); // Initialize serial port for debugging.
  delay(1000);        // Soft startup to ease the flow of electrons.

  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Button, INPUT_PULLUP);

  LEDS.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 4500); // FastLED Power management set at 5V, 4500mA.

  analogReference(INTERNAL); //1.1V

  fill_solid(leds, NUM_LEDS, b);
}

int mode = 0;
#define MODES 11 // including 0

CRGBPalette16 targetPalette;

void checkEncoder()
{
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH))
  {
    if (digitalRead(encoder0PinB) == LOW)
    {
      encoder0Pos--;
      mode--;
      if (mode < 0)
      {
        mode = MODES - 1;
      }
    }
    else
    {
      encoder0Pos++;
      mode++;
      mode = mode % MODES;
    }
    lastChangeMs = millis();
    // Serial.print(encoder0Pos);
    // Serial.print(",");
  }
  encoder0PinALast = n;
}

void checkBrightness()
{
  // brightness
  // div by 4 to convert 0-1023 to 0-255
  max_bright = analogRead(3) / 4;
  FastLED.setBrightness(max_bright);
}

void loop()
{
  checkEncoder();
  checkBrightness();

  indexFunType indexFun;
  TBlendType blendType = LINEARBLEND;
  switch (mode)
  {
  case 0:
    // Clear the strip
    blendType = NOBLEND;
    targetPalette = black_p;
    indexFun = &firstFromPaletteIndex;
    break;
  case 1:
    blendType = NOBLEND;
    targetPalette = horizonNight_p;
    indexFun = &linearIndexShiftedMidpoint;
    break;
  case 2:
    // sunrise
    blendType = NOBLEND;
    targetPalette = sunSet_p;
    indexFun = &linearIndexShiftedMidpoint;
    break;
  case 3:
    targetPalette = CloudColors_p;
    indexFun = [](int i) { return (uint8_t)(sin8(i * 16) + beat8(1)); };
    break;
  case 4:
    targetPalette = RainbowColors_p;
    indexFun = [](int i) { return (uint8_t)(sin8(i * 16) + beat8(1)); };
    break;
  case 5:
    targetPalette = RainbowColors_p;
    indexFun = [](int i) { return (uint8_t)(sin8(i * 5) + beat8(1)); };
    break;
  case 6:
    targetPalette = CloudColors_p;
    indexFun = [](int i) { return (uint8_t)(sin8(i * 16) + beat8(1)); };
    break;
  case 7:
    // sunset
    blendType = NOBLEND;
    targetPalette = sunSet_p;
    indexFun = &reverseLinearIndexShiftedMidpoint;
    break;
  case 8:
    blendType = NOBLEND;
    targetPalette = horizonNight_p;
    indexFun = &reverseLinearIndexShiftedMidpoint;
    break;
  case 9:
    // night sky with stars
    blendType = LINEARBLEND; //NOBLEND;
    targetPalette = nightPalette;
    indexFun = [](int i) {
      // or use 0 offset to turn off animation
      return (uint8_t)(sin8(i * 16) + beatsin8(1, 0, 20));
    };
    break;
  case 10:
    blendType = NOBLEND;
    targetPalette = white_p;
    indexFun = &firstFromPaletteIndex;
  default:
    break;
  }
  fillLedsFromPaletteColors(targetPalette, indexFun, blendType);
  FastLED.show(); // Power managed display
} // loop()
