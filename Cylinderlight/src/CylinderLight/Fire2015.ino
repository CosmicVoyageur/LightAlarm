#include <Arduino.h>

#define COOLING  100

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
        //leds[j] = HeatColor( heat[j]);
        byte colorindex = scale8( heat[j], 240);
        leds[j] = ColorFromPalette( currentPalette, colorindex);
    }
}



