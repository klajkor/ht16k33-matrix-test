/****************************************************************
 * Simple test of ht16k33 library turning on and off LEDs
 */

#include "Wire.h"
#include "ht16k33.h"

// Define the class
HT16K33 HT;

void I2c_Scanner(void) {
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
  delay(2000); 
  
}

void Ht16k33_Init(void) {
  HT.begin(0x70);
  HT.setBrightness(5);
  HT.clearAll();
}

/****************************************************************/
void setup() {
  Serial.begin(115200);
  // First let's do a quick scn on the I2C bus
  I2c_Scanner();
  Serial.println(F("ht16k33 light test v0.01"));
  Serial.println();
}

void Gradually_Turning_On_All_Leds(void) {
  uint8_t led;
  Serial.println(F("Turning on all 16x8 LEDs"));
  for (led=0; led<128; led++) {
    
    HT.setLedNow(led);
    delay(20);
  }
  Serial.println(F("All LEDs are on"));
}

void Brightness_Test(void) {
  uint8_t led;
  Serial.println(F("Brightness test"));
  for (led=0; led<16; led++) {
    HT.setBrightness(led);  
    delay(500);
  }
  HT.setBrightness(5);
}

void Gradually_Turning_Off_All_Leds(void) {
  uint8_t led;
  Serial.println(F("Clear all LEDs"));
  for (led=0; led<128; led++) {
    HT.clearLedNow(led);
    delay(20);
  }
  Serial.println(F("All LEDs are off"));
}

void Led_Test_One_By_One(void) {
  uint8_t led;
  for (led=0; led<128; led++) {
    HT.setLedNow(led);
    Serial.print(F("Led no "));
    Serial.print(led,DEC);
    Serial.print(F(": On"));
    delay(333);
    HT.clearLedNow(led);
    Serial.println(F(" Off"));
  }
}

/****************************************************************/
void loop() {
  Gradually_Turning_On_All_Leds();
  delay(2000);
  Brightness_Test();
  delay(2000);
  Gradually_Turning_Off_All_Leds();
  delay(2000);
  Led_Test_One_By_One();
}
    
