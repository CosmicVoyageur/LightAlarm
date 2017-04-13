#include <Arduino.h>

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
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



void checkBle(){

  /* Wait for new data to arrive */
  uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len == 0) return;

  Serial.println("got packet");
  Serial.println((char)packetbuffer[1]);
  Serial.println((char)packetbuffer[2]);
  /* Got a packet! */
   printHex(packetbuffer, len);

  // Color
  if (packetbuffer[1] == 'C') {
     handleNewColor();
  }
  //mode
  Serial.print("mode = "); Serial.println(mode);
  if(packetbuffer[1]=='B') {
    int m = packetbuffer[2] - '1';
    if( m < 4 )mode = m ;
    else
    {
      if (m==4) if(brightness < 240)  FastLED.setBrightness( brightness+=5 );  // brightness up
      if (m==5) if(brightness > 0)  FastLED.setBrightness( brightness-=5 );// brightness down
      if (m==6) if(fps > 10) fps-+5; // slower
      if (m==7) if (fps < 1000) fps+=5; // faster
    }
    
  }
  
  Serial.print("mode ");Serial.println(mode);
  Serial.print("brightness ");Serial.println(brightness);
  Serial.print("fps ");Serial.println(fps);
}

void handleNewColor(){
    uint8_t red = packetbuffer[2];
    uint8_t green = packetbuffer[3];
    uint8_t blue = packetbuffer[4];


    CRGB colour = CRGB(red,green,blue);
    int index = random8(16);
    currentPalette[index] = colour;
    Serial.print("index = "); Serial.println(index);
    for (int i = 0; i < 16; i++){
      Serial.print(currentPalette[i]);
    }
    Serial.println();
    
    Serial.print ("RGB #");
    if (red < 0x10) Serial.print("0");
    Serial.print(red, HEX);
    if (green < 0x10) Serial.print("0");
    Serial.print(green, HEX);
    if (blue < 0x10) Serial.print("0");
    Serial.println(blue, HEX);
}



