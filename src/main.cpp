
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

#define encoder0PinA 9
#define encoder0PinB 10
#define encoder0Button 11

int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

// Global variables can be changed on the fly.
// Overall brightness definition. It can be changed on the fly.
uint8_t max_bright = 128;

// Initialize LED array.
struct CRGB leds[NUM_LEDS];

uint8_t startpos = 0;
int endpos = NUM_LEDS - 1;

CRGB b = CRGB::Black;
CRGB w(20, 20, 20);
CRGBPalette16 black_p(b, b, b, b, b, b, b, b, b, b, b, b, b, b, b, b);
DEFINE_GRADIENT_PALETTE(white_p){
    0, 255, 255, 255,    // full white
    255, 255, 255, 255}; // full white

CRGBPalette16 nightPalette(b, b, b, w, b, b, b, w, b, b, b, w, b, b, b, w);

int offset = 0;
int sin8_delta = 16;
uint32_t lastChangeMs = GET_MILLIS();
TBlendType blendType = LINEARBLEND;

void fillLedsFromPaletteColors(CRGBPalette16 targetPalette)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    CRGB oldC = leds[i];
    CRGB newC = ColorFromPalette(targetPalette, sin8(i * sin8_delta) + offset, 255, blendType);
    // I could not find a fixed value to get good results,
    // the night pattern was too colorfull with low values, with high values no blending effect
    // beat8 - starts from low value in the first iterations,
    // then increases to high values to fix final effect
    leds[i] = blend(oldC, newC, beat8(15, lastChangeMs));
  }
}

void setup()
{
  Serial.begin(9600); // Initialize serial port for debugging.
  delay(1000);        // Soft startup to ease the flow of electrons.

  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Button, INPUT_PULLUP);

  LEDS.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 5000); // FastLED Power management set at 5V, 5000mA.

  analogReference(INTERNAL); //1.1V

  fill_solid(leds, NUM_LEDS, b);
}

int mode = 0;
#define MODES 6 // including 0

// for rainbow
uint8_t initialHue = 0;
// http://www.comfsm.fm/~dleeling/cis/hsl_rainbow.html
// rainbow is hue from 0 to 270
// for multiple repeated rainbows 15 was also good.
uint8_t deltahue = 270 / NUM_LEDS;

CRGBPalette16 currentPalette;
CRGBPalette16 targetPalette;

void loop()
{
  //encoder stuff
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
    lastChangeMs = GET_MILLIS();
    // Serial.print(encoder0Pos);
    // Serial.print(",");
  }
  encoder0PinALast = n;

  // brightness
  // div by 4 to convert 0-1023 to 0-255
  max_bright = analogRead(3) / 4;
  FastLED.setBrightness(max_bright);

  switch (mode)
  {
  case 0:
    // Clear the strip
    offset = 0;
    sin8_delta = 0;
    blendType = NOBLEND;
    targetPalette = black_p;
    break;
  case 1:
    offset = beat8(1, lastChangeMs);
    sin8_delta = 16;
    blendType = LINEARBLEND;
    targetPalette = CloudColors_p;
    break;
  case 2:
    offset = 0;
    sin8_delta = 16;
    blendType = LINEARBLEND;
    targetPalette = RainbowColors_p;
    break;
  case 3:
    offset = 0;
    sin8_delta = 5;
    blendType = LINEARBLEND;
    targetPalette = RainbowColors_p;
    break;
  case 4:
    // or use 0 to turn off
    offset = beatsin8(1, 0, 20, lastChangeMs);
    sin8_delta = 12;
    blendType = LINEARBLEND; //NOBLEND;
    targetPalette = nightPalette;
    break;
  case 5:
    offset = 0;
    sin8_delta = 0;
    blendType = NOBLEND;
    targetPalette = white_p;
  default:
    break;
  }
  // int maxChanges = 16;
  // nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  fillLedsFromPaletteColors(targetPalette);
  FastLED.show(); // Power managed display
  // EVERY_N_MILLISECONDS(2000)
  // {
  //   Serial.println("mode " + mode);
  // }
} // loop()
