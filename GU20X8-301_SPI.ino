#include <SPI.h>

/* 
  Shift Register latches on rising edge
  VFD on falling edge 
*/

// 74HC595 and VFD latch pin
#define latchPin PIN_SPI_SS

/*
 * VFD Pins
 */

// VFD Reset Pin
#define rstPin 8

// VFD Enable Pin
#define enPin 4

// VFD Busy Pin
#define busyPin 7

void drawAnim() {
  //static byte brightness=0;
  const byte img1[2][20]  = {
    { //blue
      0xFC, 0xFC, 0xFC, 0xF8, 0xF0,
      0xF0, 0xE0, 0xE4, 0xC0, 0x80,
      0x80, 0xC0, 0xE4, 0xE0, 0xF0,
      0xF0, 0xF8, 0xFC, 0xFC, 0xFC
    },
    { //red
      0x3F, 0x3F, 0x3F, 0x39, 0x30,
      0x31, 0x23, 0x27, 0x09, 0x03,
      0x03, 0x09, 0x27, 0x23, 0x31,
      0x30, 0x39, 0x3F, 0x3F, 0x3F
    }
  };
  const byte img2[2][20] = {
    { //blue
      0xFC, 0xFC, 0xFC, 0x9C, 0x0C,
      0x8C, 0xC4, 0xE4, 0xC8, 0x80,
      0x80, 0xC8, 0xE4, 0xC4, 0x8C,
      0x0C, 0x9C, 0xFC, 0xFC, 0xFC
    },
    { //red
      0x3F, 0x3F, 0x3F, 0x1F, 0x0F,
      0x0F, 0x07, 0x27, 0x01, 0x03,
      0x03, 0x01, 0x27, 0x07, 0x0F,
      0x0F, 0x1F, 0x3F, 0x3F, 0x3F
    }
  };
  for (int i=0; i<10; i++) {
    drawDisplay(img1);
    delay(100);
    drawDisplay(img2);
    delay(100);
  }
}

void drawName() {
  const byte nox[2][20] = {
    { //blue
      0x80, 0x80, 0x80, 0x80, 0x80,
      0x80, 0x80, 0x9C, 0xB6, 0xA2,
      0xB6, 0x9C, 0x80, 0xA2, 0x94,
      0x88, 0x94, 0xA2, 0x80, 0xFF
    },
    { //red
      0x80, 0xBE, 0x84, 0x88, 0x90,
      0xBE, 0x80, 0x9C, 0xB6, 0xA2,
      0xB6, 0x9C, 0x80, 0x80, 0x80,
      0x80, 0x80, 0x80, 0x80, 0xFF
    }
  };
  drawDisplay(nox);
  delay(1000);
}

void setBrightness(byte brightness) {
  SPI.transfer(0x3F);
  SPI.transfer(brightness);
  toggleLatch();
}

void drawBlue(const byte img[20]){
  for (byte i = 0; i<20; i++) {
    SPI.transfer(i+0x80);//address with 0x80 offset for data/control
    SPI.transfer(img[i]);
    toggleLatch();
  }
}

void drawRed(const byte img[20]){
  for (byte i=0; i<20; i++) {
    SPI.transfer(i+0x80+0x20);//address with 0x80 offset for D/C and 0x20 for red
    SPI.transfer(img[i]);
    toggleLatch();
  }
}

void drawDisplay(const byte img[2][20]){
  drawBlue(img[0]);
  drawRed(img[1]);
}

inline void toggleLatch() {
  digitalWrite(latchPin, HIGH);
  delay(1);
  digitalWrite(latchPin, LOW);
  while(digitalRead(busyPin)==HIGH);
}

void setup() {

  //disable display
  digitalWrite(enPin, LOW);
  pinMode(enPin, OUTPUT);
  
  pinMode(busyPin, INPUT);

  //setup SPI
  pinMode(latchPin, OUTPUT);
  SPI.begin();

  //reset display
  pinMode(rstPin, OUTPUT);
  digitalWrite(rstPin, HIGH);

  setBrightness(0x03);

  //enable display
  digitalWrite(enPin, HIGH);
}

void loop() {
  drawAnim();
  drawName();
}
