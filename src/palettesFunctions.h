#include "Arduino.h"
#include "FastLED.h"

// functions

typedef uint8_t (*indexFunType)(int);

uint8_t firstFromPaletteIndex(int i)
{
    return 1;
}

uint8_t linearIndex(int i)
{
    return map(i, 0, NUM_LEDS, 0, 254);
}

uint8_t reverseLinearIndex(int i)
{
    return map(i, 0, NUM_LEDS, 254, 0);
}

// from range 0-255
#define MID_POINT 42

uint8_t linearIndexShiftedMidpointCalc(uint8_t x)
{
    /* function characteristic:
  (0,positive) , eg. for MID_POINT=42 => (0,50)
  (MID_POINT,0)
  (255,255)
  demo: https://docs.google.com/spreadsheets/d/1HTOywyhYfUQjNWwb3yhnUsYyn21_lV1Gsr04tXXGE3Y
  */
    if (x < MID_POINT)
    {
        return (uint8_t)(-255.0 / (255.0 - MID_POINT) * x + (255.0 * MID_POINT) / (255.0 - MID_POINT));
    }
    else
    {
        return (uint8_t)(255.0 / (255.0 - MID_POINT) * x - (255.0 * MID_POINT) / (255.0 - MID_POINT));
    }
}

uint8_t linearIndexShiftedMidpoint(int i)
{
    return linearIndexShiftedMidpointCalc(linearIndex(i));
}

uint8_t reverseLinearIndexShiftedMidpoint(int i)
{
    return linearIndexShiftedMidpointCalc(reverseLinearIndex(i));
}

void fillLedsFromPaletteColors(CRGBPalette16 targetPalette, indexFunType indexFun, TBlendType blendType)
{
    // Serial.print(FastLED.getFPS()); // last check >200, ~240fps
    // Serial.print("/");
    EVERY_N_MILLISECONDS(33)
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
            leds[i] = blend(oldC, newC, beat8(5, lastChangeMs));
        }
    }
}
