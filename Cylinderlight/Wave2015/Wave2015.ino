#include <FastLED.h>

#define LED_PIN     7
#define COLOR_ORDER GRB
#define CHIPSET     NEOPIXEL
#define NUM_LEDS    256

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

#define BRIGHTNESS  250
#define FRAMES_PER_SECOND 20



CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);

// Params for width and height
const uint8_t ny = 8;  // kMatrixWidth
const uint8_t nx = 32; //kMatrixHeight

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;



void setup() {
  Serial.begin(115200);
  delay(3000); // sanity delay
   FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
  ChangePalettePeriodically();
  
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  Wave2015(); // run simulation frame
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


int n = 0;
void Wave2015()
{
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
      if(y == centreY) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, fx, BRIGHTNESS, currentBlending);
      else if(y == centreY+1) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, fx, BRIGHTNESS/2, currentBlending);
      else if(y == centreY-1)leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, fx, BRIGHTNESS/2, currentBlending);
      else if(y == centreY+2) leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, fx, BRIGHTNESS/4, currentBlending);
      else if(y == centreY-2)leds[XYsafe(x,y)] = ColorFromPalette( currentPalette, fx, BRIGHTNESS/4, currentBlending);
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

void ChangePalettePeriodically()
{
 // currentPalette = RainbowColors_p;         currentBlending = BLEND;
//  return;
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = BLEND; }
    if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = BLEND;  }
    if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = BLEND; }
    if( secondHand == 20)  { SetupPurpleAndGreenPalette();             currentBlending = BLEND; }
    if( secondHand == 25)  { SetupTotallyRandomPalette();              currentBlending = BLEND; }
    if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
    if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
    if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = BLEND; }
   if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = BLEND; }
//    if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND;  }
 //   if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; }
  }
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


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more 
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};





//XY stuff
uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  
  if( kMatrixSerpentineLayout == false) {
    i = (y * ny) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( x & 0x01) {
      // Odd columns run down
      uint8_t reverseY = (ny - 1) - y;
      i = (x * ny) + reverseY;
    } else {
      // Even columns run up
      i = (x* ny) + y;
    }
  }
  
  return i;
}


// actually wraps
uint16_t XYsafe( uint8_t x, uint8_t y)
{
  //Serial.print("gotx: ");  Serial.print(x);  Serial.print("  goty: ");Serial.println(y);
  
  if( x >= nx) return -1 + (nx - x); // extra wrapping
  if( y >= ny) return -1 + (ny - y);
  return XY(x,y);
}
