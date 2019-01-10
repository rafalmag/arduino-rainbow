
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

// Global variables can be changed on the fly.
// Overall brightness definition. It can be changed on the fly.
uint8_t max_bright = 128;

// Initialize LED array.
struct CRGB leds[NUM_LEDS];

// Colours defined for below
long firstval = 0xff00ff;
CRGB rgbval(50, 0, 500);
CHSV hsvval(100, 255, 200);

uint8_t startpos = 0;
int endpos = NUM_LEDS - 1;

void setup()
{
  Serial.begin(9600); // Initialize serial port for debugging.
  delay(1000);        // Soft startup to ease the flow of electrons.

  LEDS.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);

  FastLED.setBrightness(max_bright);
  set_max_power_in_volts_and_milliamps(5, 5000); // FastLED Power management set at 5V, 500mA.
}

int mode = 0;

// for rainbow
uint8_t initialHue = 0;
// http://www.comfsm.fm/~dleeling/cis/hsl_rainbow.html
// rainbow is hue from 0 to 270
// for multiple repeated rainbows 15 was also good.
uint8_t deltahue = 270 / NUM_LEDS;

void loop()
{
  switch (mode)
  {
  case 0:
    Serial.println("clear");
    // Clear the strip
    fill_solid(leds, NUM_LEDS, 0);
    break;
  case 1:
    Serial.println("rainbow");
    fill_rainbow(leds, NUM_LEDS, initialHue, deltahue);
    break;
  default:
    // will be incremented later to 0
    mode = -1;
    break;
  }
  FastLED.show(); // Power managed display
  delay(2000);
  mode = mode + 1;
  Serial.println(mode);

  // fill_solid section
  //  fill_solid(leds,NUM_LEDS, firstval);                        // A 'long' RGB value
  //  fill_solid(leds, NUM_LEDS, CRGB::HotPink);                  // https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list
  //  fill_solid(leds, NUM_LEDS, rgbval);                         // 8 bit values inside the colour definition
  //  fill_solid(leds, NUM_LEDS, hsvval);                         // 8 bit values inside the colour definition

  // fill_solid(leds, 5, rgbval);
  // fill_solid(leds+5,5, CRGB::Green);
  // fill_solid(leds+10,5, hsvval);

  // fill_gradient section
  //  fill_gradient_RGB(leds, startpos, 0x000011, endpos, 0x110000);   // You can mix and match long values and CRGB values. Remember, endpos goes up to NUM_LEDS-1
  //  fill_gradient_RGB(leds, NUM_LEDS, CRGB(50,0,200), CRGB(80,200,240));  // up to 4 CRGB (or long) values

  //FORWARD_HUES, BACKWARD_HUES, SHORTEST_HUES, LONGEST_HUES
  //  fill_gradient(leds, startpos, CHSV(50, 255,255) , endpos, CHSV(150,255,255), SHORTEST_HUES);
  //  fill_gradient(leds, NUM_LEDS, CHSV(50, 255,255), CHSV(100,255,255), LONGEST_HUES);    // up to 4 CHSV values

  // fill_rainbow section
  //  fill_rainbow(leds, NUM_LEDS, thishue, deltahue);            // Use FastLED's fill_rainbow routine.
  // fill_solid(leds, NUM_LEDS, 0); // Clear the strip for. . .
  //  fill_rainbow(leds+1, NUM_LEDS-2, thishue, deltahue);        // One pixel border at each end.

} // loop()
