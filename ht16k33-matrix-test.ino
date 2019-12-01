/****************************************************************
 * Simple test of ht16k33 library turning on and off LEDs
 */

<<<<<<< HEAD
#include "Wire.h"
=======
#include <Wire.h>
>>>>>>> 6ba1dc8460ee15c6fb3181a96e1e628f4e71fc2e
#include "ht16k33.h"

// Define the class
HT16K33 HT;

<<<<<<< HEAD
void I2c_Scanner(void) {
=======
/****************************************************************/
void setup() {
  Serial.begin(115200);
>>>>>>> 6ba1dc8460ee15c6fb3181a96e1e628f4e71fc2e
  byte error, address;
  int nDevices;

  Serial.println("I2C Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");
<<<<<<< HEAD
  delay(2000); 
  
}

/****************************************************************/
void setup() {
  Serial.begin(115200);
  // First let's do a quick scn on the I2C bus
  I2c_Scanner();
=======

  delay(2000);           // wait 5 seconds for next scan
>>>>>>> 6ba1dc8460ee15c6fb3181a96e1e628f4e71fc2e
  Serial.println(F("ht16k33 light test v0.01"));
  Serial.println();
  // initialize everything, 0x00 is the i2c address for the first one (0x70 is added in the class).
  HT.begin(0x70);
  HT.setBrightness(5);
  HT.clearAll();
  
}

/****************************************************************/
void loop() {
  uint8_t led;

  Serial.println(F("Turn on all LEDs"));
  // first light up all LEDs
  for (led=0; led<128; led++) {
    
    HT.setLedNow(led);
    delay(30);
  } // for led
  delay(2000);
  for (led=0; led<16; led++) {
    HT.setBrightness(led);  
    delay(500);
  }
  delay(2000);
  HT.setBrightness(5);
  Serial.println(F("Clear all LEDs"));
  //Next clear them
  for (led=0; led<128; led++) {
    HT.clearLedNow(led);
    delay(20);
  } // for led
  delay(2000);
  //Now do one by one, slowly, and print out which one
  for (led=0; led<128; led++) {
    
    HT.setLedNow(led);
    Serial.print(F("Led no "));
    Serial.print(led,DEC);
    Serial.print(F(": On"));
    delay(400);
    HT.clearLedNow(led);
    Serial.println(F(" Off"));
  } // for led
} // loop  
    
