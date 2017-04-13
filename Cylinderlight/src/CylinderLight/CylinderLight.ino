
/*
 * Below defines bluetooth
 * 
 */
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

//
/*
 * Below defines all the LED things
 * 
 */
#include <FastLED.h>

#define LED_PIN     7
#define COLOR_ORDER GRB
#define CHIPSET     NEOPIXEL
#define NUM_LEDS    256

#define FRAMES_PER_SECOND 20

//    -- Constants ---     //
// Params for width and height
const uint8_t ny = 8;  // kMatrixWidth
const uint8_t nx = 32; //kMatrixHeight

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;


CRGBPalette16 currentPalette;
TBlendType    currentBlending = BLEND;

CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);

int brightness = 150;


void setup() {
  Serial.begin(115200);
  setupBle();
  InitPalette();
  delay(3000); // sanity delay
   FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( brightness );
  

//ato  inputString.reserve(200); // test
}

int fps = 30;
void loop()
{
  checkBle();
  //ChangePalettePeriodically();
  
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  NextFrame();

  
  FastLED.show(); // display this frame
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
  FastLED.delay(1000 / fps);
}

int mode;
void NextFrame()
{
  switch (mode) {
    case 0: // wave 
      Wave2015();
      break;
    case 1:
      Fire2015();
      break;
    case 2:
      RotateColours();
      break;
    case 3:
      Columns();
      break;
    default: 
      // if nothing else matches, do the default
      // default is optional
    break;
  }
}




