
// for bluetooth
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#define FACTORYRESET_ENABLE     1 // enable reset when starting BLE module

// create BLE objects
SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);


// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];

                      
// for leds

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

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

//some holders for one color, testing
uint8_t red;
uint8_t green;
uint8_t blue;

// end test

void setup() {
  Serial.begin(115200);
  setupBle();
  delay(3000); // sanity delay
   FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
}

void setupBle(){

  while (!Serial);  // required for Flora & Micro
  delay(500);


  Serial.println(F("Adafruit Bluefruit Neopixel Color Picker Example"));
  Serial.println(F("------------------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  Serial.println(F("***********************"));

  // Set Bluefruit to DATA mode
  Serial.println( F("Switching to DATA mode!") );
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println(F("***********************"));

}

void loop()
{

  checkBle();
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
// test ble
  CRGB colour = CRGB(red,green,blue);
  currentPalette = CRGBPalette16( 
    colour,  colour,  colour,  colour,
    colour, colour, colour,  colour,
    colour,  colour,  colour,  colour,
    colour, colour, colour,  colour );
return;
// end test ble
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


void checkBle(){

  /* Wait for new data to arrive */
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len == 0) return;

  /* Got a packet! */
  // printHex(packetbuffer, len);

  // Color
  if (packetbuffer[1] == 'C') {
     red = packetbuffer[2];
     green = packetbuffer[3];
     blue = packetbuffer[4];
    Serial.print ("RGB #");
    if (red < 0x10) Serial.print("0");
    Serial.print(red, HEX);
    if (green < 0x10) Serial.print("0");
    Serial.print(green, HEX);
    if (blue < 0x10) Serial.print("0");
    Serial.println(blue, HEX);

   // for(uint8_t i=0; i<NUMPIXELS; i++) {
   //   pixel.setPixelColor(i, pixel.Color(red,green,blue));
   // }
    
  }
}

