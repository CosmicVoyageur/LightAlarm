int n = 0;
void Wave2015()
{
  float palletLambda =  16 / ny; 
  float lambda = 256.0/31;
//  Serial.println("Wave");
//  Serial.print("n: "); Serial.println(n);
  for(int x = nx - 1; x >=0; x--)
  {
    uint8_t fx = lambda * ((x + n)%nx);
    uint8_t sinResult = sin8( fx);
    int centreY = (sinResult - 1) * 8 / 256; // because sin8(i) returns between 0 - 256
    for(int y = ny - 1; y >= 0; y--)
    {
      if(y == centreY) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda * y, BRIGHTNESS, currentBlending);
      else if(y == centreY+1) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda * y, BRIGHTNESS/2, currentBlending);
      else if(y == centreY-1)leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda*y, BRIGHTNESS/2, currentBlending);
      else if(y == centreY+2) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda*y, BRIGHTNESS/4, currentBlending);
      else if(y == centreY-2)leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, palletLambda*y, BRIGHTNESS/4, currentBlending);
      else leds[XYsafe(x,y)] = CRGB::Black;
    }
    

//Serial.print("  lambda:" );
//Serial.print(lambda);
//Serial.print("  centre y:" );
//Serial.print(centreY);
//Serial.print("  fx: ");
//Serial.println(fx);
//Serial.print("  sinResult: ");
//Serial.print(sinResult);

//    leds[XYsafe(x,centreY)] = ColorFromPalette( currentPalette, n, BRIGHTNESS, currentBlending);
  }
  
  n++;
  if(n==32) n = 0;
//Serial.println();
if(n==0) Serial.println("boop");
}

