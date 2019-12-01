/****************************************************************
 * Simple test of ht16k33 library turning on and off LEDs
 */

#include <Arduino.h>
#include <Wire.h>
#include <ht16k33.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>


// X mirror class start
#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

class Adafruit_X_Mirrored_8x8matrix: public Adafruit_LEDBackpack, public Adafruit_GFX {
  
  public:
  Adafruit_X_Mirrored_8x8matrix(void);

  void drawPixel(int16_t x, int16_t y, uint16_t color);

  private:

};

Adafruit_X_Mirrored_8x8matrix::Adafruit_X_Mirrored_8x8matrix(void) : Adafruit_GFX(8, 8) {
}

void Adafruit_X_Mirrored_8x8matrix::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((y < 0) || (y >= 8)) return;
  if ((x < 0) || (x >= 8)) return;

 // check rotation, move pixel around if necessary
  switch (getRotation()) {
  case 1:
    _swap_int16_t(x, y);
    x = 8 - x - 1;
    break;
  case 2:
    x = 8 - x - 1;
    y = 8 - y - 1;
    break;
  case 3:
    _swap_int16_t(x, y);
    y = 8 - y - 1;
    break;
  }

  x = 8 - x - 1; // X pixel mirrored

    if (color) {
    displaybuffer[y] |= 1 << x;
  } else {
    displaybuffer[y] &= ~(1 << x);
  }
}

// X mirror class end

// define Adafruit library class
Adafruit_X_Mirrored_8x8matrix matrix = Adafruit_X_Mirrored_8x8matrix();


static const uint8_t PROGMEM
  smile_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10100101,
    B10011001,
    B01000010,
    B00111100 },
  neutral_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10111101,
    B10000001,
    B01000010,
    B00111100 },
  frown_bmp[] =
  { B00111100,
    B01000010,
    B10100101,
    B10000001,
    B10011001,
    B10100101,
    B01000010,
    B00111100 };

// Define lpaseen library class
HT16K33 HT;

void I2c_Scanner(void) {
  byte error, address;
  int nDevices;

  Serial.println("I2C Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print(F("I2C device found at address 0x"));
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

void Adafruit_Ht_Matrix_init() {
  matrix.begin(0x70);
}

/****************************************************************/
void setup() {
  Serial.begin(115200);
  // First let's do a quick scan on the I2C bus
  I2c_Scanner();
  Ht16k33_Init();
  Adafruit_Ht_Matrix_init();
  Serial.println(F("HT16K33 light test v1.0"));
  Serial.println();
}

void Gradually_Turning_On_All_Leds(void) {
  uint8_t led;
  Serial.println(F("Turning on all 16x8 LEDs"));
  for (led=0; led<128; led++) {
    
    HT.setLedNow(led);
    delay(16);
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
    delay(16);
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
    delay(200);
    HT.clearLedNow(led);
    Serial.println(F(" Off"));
  }
}

void Draw_Faces(void) {
  Serial.println(F("Smile"));
  matrix.clear();
  matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_ON);
  matrix.drawBitmap(0, 8, smile_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(1500);
  Serial.println(F("Neutral"));
  matrix.clear();
  matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_ON);
  matrix.drawBitmap(0, 8, neutral_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(1500);
  Serial.println(F("Frown"));
  matrix.clear();
  matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_ON);
  matrix.drawBitmap(0, 8, frown_bmp, 8, 8, LED_ON);
  matrix.writeDisplay();
  delay(1500);
}

void Adafruit_Text_Display_Test() {
  int8_t x,y;
  matrix.setRotation(0);
  Serial.println(F("Scrolling, rotation=0"));
  matrix.setTextColor(LED_ON);
  for (y=0; y<=1; y++) {
    for (x=7; x>=-77; x--) {
      matrix.clear();
      matrix.setCursor(x,1);
      matrix.print("- Hello World");
      matrix.writeDisplay();
      delay(60);
    }
  }
  delay(2000);
  Serial.println(F("Scrolling, rotation=1"));
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setRotation(1);
  for (y=0; y<=1; y++) {
    for (x=7; x>=-77; x--) {
      matrix.clear();
      matrix.setCursor(x,1);
      matrix.print("- Hello World");
      matrix.writeDisplay();
      delay(60);
    }
  }
  matrix.setRotation(0);
  
}

void Line_test(void) {
  int8_t x,y,rotation;
  matrix.clear();
  matrix.writeDisplay();
  delay(500);
  for (rotation=0; rotation<=3; rotation++) {
    matrix.setRotation(rotation);
    for (y=0; y<=7; y++) {
      matrix.clear();
      Serial.print(F("Rotation: "));
      Serial.print(rotation);
      Serial.print(F(" "));
      Serial.print(F("Y: "));
      Serial.print(y);
      Serial.println(F(""));
      Serial.print(F("X: "));
      for (x=0; x<=7; x++) {
        Serial.print(x);
        Serial.print(F(" "));
        matrix.drawPixel(x, y, LED_ON);
        matrix.writeDisplay();
        delay(300);
      }
      Serial.println(F(" "));
    }
  }
}

/****************************************************************/
void loop() {
  matrix.setRotation(0);
  Draw_Faces();
  matrix.setRotation(1);
  Draw_Faces();
  matrix.setRotation(0);
  Line_test();
  Adafruit_Text_Display_Test();
  delay(2000);
  
  Gradually_Turning_On_All_Leds();
  delay(2000);
  
  Brightness_Test();
  delay(2000);
  Gradually_Turning_Off_All_Leds();
  delay(2000);
  Led_Test_One_By_One();
  
}
    
