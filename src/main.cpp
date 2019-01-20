
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
DEFINE_GRADIENT_PALETTE(black_gp){
    0, 0, 0, 0,    // full black
    255, 0, 0, 0}; // full black
CRGBPalette16 black_p = black_gp;

DEFINE_GRADIENT_PALETTE(white_gp){
    0, 255, 255, 255,    // full white
    255, 255, 255, 255}; // full white
CRGBPalette16 white_p = white_gp;

//mine demo test
DEFINE_GRADIENT_PALETTE(morning1_gp){
    0, 255, 0, 0,      // red
    255, 255, 255, 0}; // yellow
CRGBPalette16 morning1_p = morning1_gp;
DEFINE_GRADIENT_PALETTE(morning2_gp){
    0, 255, 0, 0,    // red
    255, 0, 0, 255}; // blue
CRGBPalette16 morning2_p = morning2_gp;

CRGB b = CRGB::Black;
CRGB star(20, 20, 20);
CRGBPalette16 nightPalette(b, b, b, star, b, b, b, star, b, b, b, star, b, b, b, star);

// below palletes based on
/*
#sun {
  background: -webkit-radial-gradient(bottom, circle, rgba(242,248,247,1) 0%,rgba(249,249,28,1) 3%,rgba(247,214,46,1) 8%, rgba(248,200,95,1) 12%,rgba(201,165,132,1) 30%,rgba(115,130,133,1) 51%,rgba(46,97,122,1) 85%,rgba(24,75,106,1) 100%);
  opacity: 0.5;
*/

DEFINE_GRADIENT_PALETTE(sun_gp){
    0, 242, 248, 247,
    8, 249, 249, 28,    // 3%
    20, 247, 214, 46,   // 8%
    30, 248, 200, 95,   // 12%
    77, 201, 165, 132,  // 30%
    130, 115, 130, 133, // 51%
    217, 46, 97, 122,   // 85%
    255, 24, 75, 106};
CRGBPalette16 sun_p = sun_gp;

/*
#sunDay {
  background: -webkit-radial-gradient(bottom, circle, rgba(252,255,251,0.9) 0%,rgba(253,250,219,0.4) 30%,rgba(226,219,197,0.01) 70%, rgba(226,219,197,0.0) 70%,rgba(201,165,132,0) 100%);
 opacity: 0.5;
 */

DEFINE_GRADIENT_PALETTE(sunDay_gp){
    0, 252, 255, 251,
    77, 253, 250, 219,  // 30%
    179, 226, 219, 197, // 70%
    255, 201, 165, 132};
CRGBPalette16 sunDay_p = sunDay_gp;

/*
#moon {
  background: -webkit-radial-gradient(40% 55%, circle, rgba(249,249,250,1) -1%, rgba(189,255,254,1) 1%, rgba(8,49,78,1) 1%, rgba(8,26,56,1) 10%, rgba(4,16,46,1) 40%, rgba(2,8,13,1) 70%);
  opacity: 0;  
*/

/*
#sunSet {
    background: -webkit-radial-gradient(bottom, circle, rgba(254,255,255,0.8) 5%,rgba(236,255,0,1) 10%,rgba(253,50,41,1) 25%, rgba(243,0,0,1) 40%,rgba(93,0,0,1) 100%);
     opacity: 0.3;
*/
DEFINE_GRADIENT_PALETTE(sunSet_gp){
    0, 254, 255, 255,
    13, 254, 255, 255, // 5%
    26, 236, 255, 0,   // 10%
    64, 253, 50, 41,   // 25%
    102, 243, 0, 0,    // 40%
    255, 93, 0, 0};
CRGBPalette16 sunSet_p = sunSet_gp;

/*
#sky {
   background: -webkit-linear-gradient(bottom, rgba(249,251,240,1) 10%, rgba(215,253,254,1) 20%, rgba(167,222,253,1) 40%, rgba(110,175,255,1) 100%); 
   opacity: 0.52;
*/
DEFINE_GRADIENT_PALETTE(sky_gp){
    0, 249, 251, 240,
    26, 249, 251, 240,  // 10%
    51, 215, 253, 254,  // 20%
    102, 167, 222, 253, // 40%
    255, 110, 175, 255};
CRGBPalette16 sky_p = sky_gp;

/*
#horizonNight {
  background: -webkit-linear-gradient(bottom, rgba(57,167,255,1) 0%, rgba(13,98,245,1) 20%,rgba(0,11,22,0.1) 60%);
  opacity: 0;
*/
// reduced brightness by roughly 65%
DEFINE_GRADIENT_PALETTE(horizonNight_gp){
    0, 37, 108, 165, // 0%
    51, 8, 66, 166,  // 20%
    153, 0, 9, 18,   // 60%
    255, 0, 0, 0};
CRGBPalette16 horizonNight_p = horizonNight_gp;

unsigned long lastChangeMs = millis();

typedef uint8_t (*indexFunType)(int);

uint8_t linearIndex(int i)
{
  return map(i, 0, NUM_LEDS, 0, 254);
}

uint8_t firstFromPaletteIndex(int i)
{
  return 1;
}

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
#define MODES 9 // including 0

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
    lastChangeMs = millis();
    // Serial.print(encoder0Pos);
    // Serial.print(",");
  }
  encoder0PinALast = n;

  // brightness
  // div by 4 to convert 0-1023 to 0-255
  max_bright = analogRead(3) / 4;
  FastLED.setBrightness(max_bright);

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
    // sunrise
    if (millis() - lastChangeMs <= 100)
    {
      targetPalette = horizonNight_p;
    }
    else
    {
      nblendPaletteTowardPalette(targetPalette, sunSet_p, 2);
    }
    indexFun = &linearIndex;
    break;
  case 2:
    targetPalette = CloudColors_p;
    indexFun = [](int i) { return (uint8_t)(sin8(i * 16) + beat8(1)); };
    break;
  case 3:
    targetPalette = RainbowColors_p;
    indexFun = [](int i) { return sin8(i * 16); };
    break;
  case 4:
    targetPalette = RainbowColors_p;
    indexFun = [](int i) { return sin8(i * 5); };
    break;
  case 5:
    if (millis() - lastChangeMs <= 100)
    {
      targetPalette = sky_p;
    }
    else
    {
      nblendPaletteTowardPalette(targetPalette, sunSet_p, 2);
    }
    indexFun = &linearIndex;
    break;
  case 6:
    if (millis() - lastChangeMs <= 100)
    {
      targetPalette = sunSet_p;
    }
    else
    {
      nblendPaletteTowardPalette(targetPalette, horizonNight_p, 2);
    }
    indexFun = &linearIndex;
    break;
  case 7:
    // night sky with stars
    blendType = LINEARBLEND; //NOBLEND;
    targetPalette = nightPalette;
    indexFun = [](int i) {
      // or use 0 offset to turn off animation
      return (uint8_t)(sin8(i * 16) + beatsin8(1, 0, 20));
    };
    break;
  case 8:
    blendType = NOBLEND;
    targetPalette = white_p;
    indexFun = &firstFromPaletteIndex;
  default:
    break;
  }
  // int maxChanges = 16;
  fillLedsFromPaletteColors(targetPalette, indexFun, blendType);
  FastLED.show(); // Power managed display
  // EVERY_N_MILLISECONDS(2000)
  // {
  //   Serial.println("mode " + mode);
  // }
} // loop()
