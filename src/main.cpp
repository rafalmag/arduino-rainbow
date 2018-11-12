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

void loop()
{
  // put your main code here, to run repeatedly:
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(1000);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(1000);

  int r = analogRead(0) /4;
  int g = analogRead(1) /4;
  int b = analogRead(2) /4;
  int perc = analogRead(3);

  // Serial.print("rgb(");
  // Serial.print(r);
  // Serial.print(",");
  // Serial.print(g);
  // Serial.print(",");
  // Serial.print(b);
  // Serial.print("), proc=");
  // Serial.print(perc);
  // Serial.println();

  for (int i = 0; i < NUMPIXELS; i++)
  {
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  delay(100);
}