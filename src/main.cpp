#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  // put your setup code here, to run once:
  analogReference(INTERNAL); //1.1V
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  pixels.begin(); // This initializes the NeoPixel library.
}

void setLeds(int listSize, int r, int g, int b, float perc)
{
  float mid = (listSize - 1) * perc;

  // left
  float r_a1 = r / mid;
  float g_a1 = g / mid;
  float b_a1 = b / mid;
  for (int i = 0; i < mid; i++)
  {
    int r_val = i * r_a1;
    int g_val = i * g_a1;
    int b_val = i * b_a1;
    pixels.setPixelColor(i, pixels.Color(r_val, g_val, b_val));
  }
  //right
  float r_a2 = (255 - r) / mid;
  float g_a2 = (255 - g) / mid;
  float b_a2 = (255 - b) / mid;
  int r_b2 = 2 * r - 255;
  int g_b2 = 2 * g - 255;
  int b_b2 = 2 * b - 255;
  for (int i = mid + 1; i < listSize; i++)
  {
    int r_val = i * r_a2 + r_b2;
    int g_val = i * g_a2 + g_b2;
    int b_val = i * b_a2 + b_b2;
    pixels.setPixelColor(i, pixels.Color(r_val, g_val, b_val));
  }
}

void loop()
{
  int r = analogRead(0) / 4;
  int g = analogRead(1) / 4;
  int b = analogRead(2) / 4;
  float perc = analogRead(3) / 1023.0;

  setLeds(NUMPIXELS, r, g, b, perc);
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(100);
}