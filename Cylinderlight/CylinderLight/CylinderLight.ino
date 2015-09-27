#include <FastLED.h>

#define LED_PIN     7
#define COLOR_ORDER GRB
#define CHIPSET     NEOPIXEL
#define NUM_LEDS    256
#define BRIGHTNESS  250
#define FRAMES_PER_SECOND 20

//    -- Constants ---     //
// Params for width and height
const uint8_t ny = 8;  // kMatrixWidth
const uint8_t nx = 32; //kMatrixHeight

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = true;


CRGBPalette16 currentPalette;
TBlendType    currentBlending;





CRGB leds_plus_safety_pixel[ NUM_LEDS + 1];
CRGB* leds( leds_plus_safety_pixel + 1);


String inputString ;      // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  Serial.print("got - ");Serial.println(inputString);
  inputString == "";
  stringComplete=false;
}

void setup() {
  Serial.begin(115200);
  delay(3000); // sanity delay
   FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );

  inputString.reserve(200); // test
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





