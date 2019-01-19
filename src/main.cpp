
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
CRGBPalette16 nightPalette(b, b, b, w, b, b, b, w, b, b, b, w, b, b, b, w);

void fillLedsFromPaletteColors(CRGBPalette16 targetPalette)
{

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = ColorFromPalette(targetPalette, i + sin8(i * 16), 255);
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
}

int mode = 0;
#define MODES 5 // including 0

// for rainbow
uint8_t initialHue = 0;
// http://www.comfsm.fm/~dleeling/cis/hsl_rainbow.html
// rainbow is hue from 0 to 270
// for multiple repeated rainbows 15 was also good.
uint8_t deltahue = 270 / NUM_LEDS;

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
      if(mode<0){
        mode = MODES -1;
      }
    }
    else
    {
      encoder0Pos++;
      mode++;
      mode = mode % MODES;
    }
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
    fill_solid(leds, NUM_LEDS, 0);
    break;
  case 1:
    fillLedsFromPaletteColors(CloudColors_p);
    break;
  case 2:
    fill_rainbow(leds, NUM_LEDS, initialHue, deltahue);
    break;
  case 3:
    fill_rainbow(leds, NUM_LEDS, initialHue, 15);
    break;
  case 4:
    fillLedsFromPaletteColors(nightPalette);
    break;
  default:
    break;
  }
  FastLED.show(); // Power managed display
  EVERY_N_MILLISECONDS(2000)
  {
    mode = mode + 1;
    Serial.println("mode " + mode);
  }
} // loop()
