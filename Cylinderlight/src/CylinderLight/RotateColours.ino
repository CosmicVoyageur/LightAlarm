#include <Arduino.h>


uint8_t colorIndex = 0;
void RotateColours()
{
  uint8_t colorOffset = colorIndex++; /* motion speed */

  ChangePalettePeriodically();
  
 // for( int i = 0; i < NUM_LEDS; i++) {
  //  leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
  
  //  colorIndex += 3;
 // }
  for ( int i=0; i < nx; i++)
  {
    for (int j=0; j < ny; j++)
    {
      leds[XYsafe(i,j)] = ColorFromPalette( currentPalette, colorOffset, brightness, currentBlending);
    }
    colorOffset += 3;
  }
  
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = BLEND; }
    if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = BLEND;  }
    if( secondHand == 20)  { currentPalette = RainbowStripeColors_p;   currentBlending = BLEND; }
    if( secondHand == 30)  { SetupPurpleAndGreenPalette();             currentBlending = BLEND; }
    if( secondHand == 40)  { SetupTotallyRandomPalette();              currentBlending = BLEND; }
  //  if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
 //   if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
 //   if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = BLEND; }
 //  if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = BLEND; }
  //  if( secondHand == 50)  { currentPalette = wavePallet_p; currentBlending = BLEND;  }
 //   if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; }
  }
}

