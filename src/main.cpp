
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

// palettes
DEFINE_GRADIENT_PALETTE(black_p){
    0, 0, 0, 0,    // full black
    255, 0, 0, 0}; // full black
DEFINE_GRADIENT_PALETTE(white_p){
    0, 255, 255, 255,    // full white
    255, 255, 255, 255}; // full white

CRGB b = CRGB::Black;
CRGB star(20, 20, 20);
CRGBPalette16 nightPalette(b, b, b, star, b, b, b, star, b, b, b, star, b, b, b, star);

uint32_t lastChangeMs = GET_MILLIS();

typedef uint8_t (*indexFunType)(int);

void fillLedsFromPaletteColors(CRGBPalette16 targetPalette, indexFunType indexFun, TBlendType blendType)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    CRGB oldC = leds[i];
    // sin8(i * sin8_delta) + offset
    CRGB newC = ColorFromPalette(targetPalette, indexFun(i), 255, blendType);
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
  set_max_power_in_volts_and_milliamps(5, 4500); // FastLED Power management set at 5V, 4500mA.

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

  indexFunType indexFun;
  TBlendType blendType;
  switch (mode)
  {
  case 0:
    // Clear the strip
    blendType = NOBLEND;
    targetPalette = black_p;
    indexFun = [](int i) { return (uint8_t)1; };
    break;
  case 1:
    blendType = LINEARBLEND;
    targetPalette = CloudColors_p;
    indexFun = [](int i) { return (uint8_t)(sin8(i * 16) + beat8(1)); };
    break;
  case 2:
    blendType = LINEARBLEND;
    targetPalette = RainbowColors_p;
    indexFun = [](int i) { return sin8(i * 16); };
    break;
  case 3:
    blendType = LINEARBLEND;
    targetPalette = RainbowColors_p;
    indexFun = [](int i) { return sin8(i * 5); };
    break;
  case 4:
    blendType = LINEARBLEND; //NOBLEND;
    targetPalette = nightPalette;
    indexFun = [](int i) {
      // or use 0 offset to turn off animation
      return (uint8_t)(sin8(i * 16) + beatsin8(1, 0, 20));
    };
    break;
  case 5:
    blendType = NOBLEND;
    targetPalette = white_p;
    indexFun = [](int i) { return (uint8_t)1; };
  default:
    break;
  }
  // int maxChanges = 16;
  // nblendPaletteTowardPalette(currentPalette, targetPalette, maxChanges);
  fillLedsFromPaletteColors(targetPalette, indexFun, blendType);
  FastLED.show(); // Power managed display
  // EVERY_N_MILLISECONDS(2000)
  // {
  //   Serial.println("mode " + mode);
  // }
} // loop()
