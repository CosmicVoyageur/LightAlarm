int n = 0;

void Wave2015()
{
  float lambda = 256.0/31;

  // Loop through X
  for(int x = nx - 1; x >=0; x--)
  {
    uint8_t fx = lambda * ((x + n)%nx);
    uint8_t sinResult = sin8( fx);
    int centreY = (sinResult - 1) * 8 / 256; // because sin8(i) returns between 0 - 256

    // Loop through Y
    for(int y = ny - 1; y >= 0; y--)
    {
      if(y == centreY) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, x * 256/nx, brightness, currentBlending);
      else if(y == centreY+1) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, x * 256/nx, brightness/2, currentBlending);
      else if(y == centreY-1)leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, x * 256/nx, brightness/2, currentBlending);
      else if(y == centreY+2) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, x * 256/nx, brightness/4, currentBlending);
      else if(y == centreY-2)leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, x * 256/nx, brightness/4, currentBlending);
      else leds[XYsafe(x,y)] = CRGB::Black;
    } // end Loop-Y
  } // End Loop-X
  n++;
  if(n==32) n = 0; // n roll-over
}



