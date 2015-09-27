

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
