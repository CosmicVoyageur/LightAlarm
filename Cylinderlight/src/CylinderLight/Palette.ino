#include <Arduino.h>

extern const TProgmemPalette16 wavePallet_p PROGMEM;



void InitPalette(){
  SetupWaterPalette();
}


// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}
// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}


void SetupWaterPalette()
{
  
  CRGB darkerBlue = 0x0B132B;
  CRGB darkBlue = 0x1C2541;
  CRGB greyBlue = 0x3A506B;
  CRGB blue = 0x5BC0BE;
  CRGB lightBlue = 0x6FFFE9;


  
  currentPalette = CRGBPalette16( 
    darkerBlue,  darkerBlue,  darkerBlue, darkerBlue, 
    darkBlue, darkBlue, darkBlue,  darkBlue, 
    greyBlue,  greyBlue,  greyBlue, greyBlue, 
    lightBlue, lightBlue, lightBlue,  lightBlue );
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more 
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 wavePallet_p PROGMEM =
{
  CRGB::Blue,
  CRGB::Blue, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Blue,

  CRGB::Blue,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Blue,

  CRGB::Gray,
  CRGB::Blue,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Gray,
  CRGB::Gray
};

