#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 30

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
int iter = 0;

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
  if (perc < 0.0001)
  {
    // to get rid of div by 0 issues
    perc = 0.0001;
  }

  float mid = (listSize - 1) * perc;

  // left
  float r_a1 = r / mid;
  float g_a1 = g / mid;
  float b_a1 = b / mid;
  for (int i = 0; i <= mid; i++)
  {
    int r_val = i * r_a1;
    int g_val = i * g_a1;
    int b_val = i * b_a1;
    pixels.setPixelColor(i, pixels.Color(r_val, g_val, b_val));
  }
  //right
  float divider = (listSize - 1 - mid);

  int r_b2 = 100 - r;
  int g_b2 = 100 - g;
  int b_b2 = 100 - b;

  for (int i = mid + 1; i < listSize; i++)
  {
    float i_factor = i - mid;
    int r_val = r_b2 * i_factor / divider + r;
    int g_val = g_b2 * i_factor / divider + r;
    int b_val = b_b2 * i_factor / divider + r;

    pixels.setPixelColor(i, pixels.Color(r_val, g_val, b_val));
  }
}

void loop()
{
  int r = analogRead(0) * 0.09775;
  int g = analogRead(1) * 0.09775;
  int b = analogRead(2) * 0.09775;
  float perc = analogRead(3) / 1023.0;

  // TODO alternative idea, use 0-255
  // pixels.setBrightness(perc * 255);

  setLeds(NUMPIXELS, r, g, b, perc);
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(100);

  if (iter % 15 == 0)
  {
    iter = 0;
    for (int i = 0; i < NUMPIXELS; i++)
    {
      Serial.print(i);
      Serial.print(": ");

      uint32_t c = pixels.getPixelColor(i);
      uint8_t r = (uint8_t)(c >> 16);
      uint8_t g = (uint8_t)(c >> 8);
      uint8_t b = (uint8_t)c;
      Serial.print(r);
      Serial.print(",");
      Serial.print(g);
      Serial.print(",");
      Serial.print(b);
      Serial.println();
    }
    Serial.println();
  }
  iter++;
}