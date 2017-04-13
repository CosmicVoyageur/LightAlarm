#include <Arduino.h>


void Columns()
{
  int colorOffset = 0;
  CRGB color = color = ColorFromPalette( currentPalette, colorOffset, brightness, currentBlending);
  for ( int i=0; i < nx; i++) // there are 32 x and 16 in palette
  {
    color = ColorFromPalette( currentPalette, colorOffset, brightness, currentBlending);
    for (int j=0; j < ny; j++)
    {
      leds[XYsafe(i,j)] = color;
    }
    colorOffset += 256/32;
  }
}

