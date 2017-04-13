#include <Arduino.h>

#include <FastLED.h>

#define LED_PIN     7
#define COLOR_ORDER GRB
#define CHIPSET     NEOPIXEL
#define NUM_LEDS    256

#define BRIGHTNESS  250
#define FRAMES_PER_SECOND 60


CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);


//CRGB leds[NUM_LEDS];

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
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  Fire2015(); // run simulation frame
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  100

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 200


void Fire2015()
{
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
 // Serial.println("Step 1");
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }
 // Serial.println("Step 2");
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int x= nx - 1; x>= 0; x--) {
       for( int y= ny - 1; y>= 1; y--) {
      //heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
        int k = XYsafe(x,y);int k0 = XYsafe(x-1,y);int k01 = XYsafe(x+1,y); int k1 = XYsafe(x,y-1); int k11 = XYsafe(x+1,y-1);int k12 = XYsafe(x-1,y-1);int k2 = XYsafe(x,y-2); 
        heat[k] = (2 * heat[k1] + heat[k0]+ heat[k01] + heat[k11] + heat[k12] + 2 * heat[k2] ) / ( 3 * (y+1));
       }
    }
 //   Serial.println("Step 3");
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int x = random8(0, nx);
      int y = random8(0,1);
      int q = XYsafe(x,y);
      heat[q] = qadd8( heat[q], random8(160,255) );
    }
//   Serial.println("Step 4");
    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
        leds[j] = HeatColor( heat[j]);
    }
}


//
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
