int n = 0;

void Wave2015()
{
  float palletLambda =  16 / ny; 
  float lambda = 256.0/31;

  // Loop through X
  for(int x = nx - 1; x >=0; x--)
  {
    Serial.print("x: "); Serial.println(x);
    Serial.print("(x + n)%nx : "); Serial.println((x + n)%nx);
    uint8_t fx = lambda * ((x + n)%nx);
    uint8_t sinResult = sin8( fx);
    int centreY = (sinResult - 1) * 8 / 256; // because sin8(i) returns between 0 - 256

    // Loop through Y
    for(int y = ny - 1; y >= 0; y--)
    {
      if(y == centreY) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda * y, BRIGHTNESS, currentBlending);
      else if(y == centreY+1) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda * y, BRIGHTNESS/2, currentBlending);
      else if(y == centreY-1)leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda*y, BRIGHTNESS/2, currentBlending);
      else if(y == centreY+2) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda*y, BRIGHTNESS/4, currentBlending);
      else if(y == centreY-2)leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda*y, BRIGHTNESS/4, currentBlending);
      else leds[XYsafe(x,y)] = CRGB::Black;
    } // end Loop-Y
  } // End Loop-X
  n++;
  if(n==32) n = 0; // n roll-over
}



