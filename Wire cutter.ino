
#include <Adafruit_GFX.h>   
#include <SPI.h>
#include <Adafruit_ILI9341.h>
#include "TouchScreen.h"

#define YP A2
#define XM A3
#define YM 7
#define XP 6

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

#define MINPRESSURE 10
#define MAXPRESSURE 1000

// The display uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

const int stepPinE = 5;    // extruder motor
const int dirPinE = 4;
const int stepPinC = 3;    // cutting motor
const int dirPinC = 8;

int units = 0;

int page = 1;

int wireLength = 0;

int wireCount = 0;

int steps = 0;        // depends on unit selected

void setup(void) {

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), runout, FALLING);

  Serial.begin(9600);
  Serial.println(F("Touch Paint!"));

  pinMode(stepPinE, OUTPUT);
  pinMode(dirPinE, OUTPUT);
  pinMode(stepPinC, OUTPUT);
  pinMode(dirPinC, OUTPUT);

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  tft.setCursor(45, 15);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);

  tft.println("AUTOMATIC");
  tft.setCursor(25, 50);
  tft.println("WIRE CUTTER");

  tft.setTextSize(2);
  tft.setCursor(25, 100);
  tft.println("Prabhat Adhikari");
  tft.setCursor(34, 130);
  tft.println("Tom Harrington");

  tft.setCursor(17, 160);
  tft.setTextColor(ILI9341_CYAN);
  tft.println("MEMS 1041 Group 31");

  tft.drawRoundRect(55,  230,  140,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(56,  231, 138 , 53, 8, ILI9341_GREEN);
  tft.setCursor(69, 248);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("START>");
}

void loop()
{
  // Retrieve a point
  TSPoint p = ts.getPoint();

  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
    return;
  }

  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

  if (page == 1) {

    p = ts.getPoint();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    if (p.x > 50 && p.x < 200 && p.y > 230 && p.y < 290) {
      tft.fillScreen(ILI9341_BLACK);
      drawPage2();
      delay(500);
      page = 2;
    }
  }

  if (page == 2) {

    p = ts.getPoint();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    if (p.x > 20 && p.x < 120 && p.y > 200 && p.y < 290) {
      units = 0;
      tft.fillScreen(ILI9341_BLACK);
      drawPage3Inch();
      delay(500);
      page = 3;
    }
    else if (p.x > 130 && p.x < 230 && p.y > 200 && p.y < 290) {
      units = 1;
      tft.fillScreen(ILI9341_BLACK);
      drawPage3Cm();
      delay(500);
      page = 3;
    }
  }

  if (page == 3) {

    p = ts.getPoint();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    if (p.x > 155 && p.x < 225 && p.y > 100 && p.y < 165) {
      if (units == 0) {
        wireLength++;
        if (wireLength > 10) wireLength = 10;
      }
      else if (units == 1) {
        wireLength++;
        if (wireLength > 25) wireLength = 25;
      }
      tft.fillRoundRect(35,  140, 80 , 50, 2, ILI9341_BLACK);
      tft.setCursor(40, 145);
      tft.setTextSize(4);
      tft.print(wireLength);
      delay(100);
    }
    else if (p.x > 155 && p.x < 225 && p.y > 175 && p.y < 230) {
      wireLength--;
      if (wireLength < 0) wireLength = 0;
      tft.fillRoundRect(35,  140, 80 , 50, 2, ILI9341_BLACK);
      tft.setCursor(40, 145);
      tft.setTextSize(4);
      tft.print(wireLength);
      delay(100);
    }
    else if (p.x > 135 && p.x < 235 && p.y > 250 && p.y < 310) {
      tft.fillScreen(ILI9341_BLACK);
      drawPage4();
      delay(500);
      page = 4;

    }

  }
  if (page == 4) {
    p = ts.getPoint();
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());

    if (p.x > 155 && p.x < 225 && p.y > 100 && p.y < 165) {
      wireCount++;
      if (wireCount > 20) wireCount = 20;
      tft.fillRoundRect(35,  140, 80 , 50, 2, ILI9341_BLACK);
      tft.setCursor(40, 145);
      tft.setTextSize(4);
      tft.print(wireCount);
      delay(100);
    }
    else if (p.x > 155 && p.x < 225 && p.y > 175 && p.y < 230) {
      wireCount--;
      if (wireCount < 0) wireCount = 0;
      tft.fillRoundRect(35,  140, 80 , 50, 2, ILI9341_BLACK);
      tft.setCursor(40, 145);
      tft.setTextSize(4);
      tft.print(wireCount);
      delay(100);
    }
    else if (p.x > 100 && p.x < 235 && p.y > 250 && p.y < 310) {
      tft.fillScreen(ILI9341_BLACK);
      drawPage5();
      delay(500);
      page = 5;

    }
  }
  if (page == 5) {
    cutWire(wireLength, wireCount, units);
    delay (1000);
    page = 1;
  }

}

void drawPage2() {
  tft.setCursor(10, 40);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(3);
  tft.println("Select units  of length");

  tft.drawRoundRect(25,  200,  90,  90,  8, ILI9341_WHITE);
  tft.fillRoundRect(26,  201, 88 , 88 , 8, ILI9341_GREEN);
  tft.setCursor(35, 235);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("INCH");

  tft.drawRoundRect(135,  200,  90,  90,  8, ILI9341_WHITE);
  tft.fillRoundRect(136,  201, 88 , 88 , 8, ILI9341_GREEN);
  tft.setCursor(165, 235);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("CM");

}

void drawPage3Inch() {
  tft.setCursor(20, 10);
  tft.setTextSize(2);
  tft.println("Enter length of      wire in inches");
  tft.println();
  tft.println(" 1 inch increments,    10 inches max");

  tft.setCursor(40, 145);
  tft.setTextSize(4);
  tft.print(wireLength);

  tft.drawRoundRect(155,  110,  70,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(156,  111, 68 , 53 , 8, ILI9341_CYAN);
  tft.setCursor(180, 125);
  tft.setTextSize(4);
  tft.print("+");

  tft.drawRoundRect(155,  175,  70,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(156,  176, 68 , 53 , 8, ILI9341_CYAN);
  tft.setCursor(180, 190);
  tft.setTextSize(4);
  tft.print("-");

  tft.drawRoundRect(135,  250,  100,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(136,  251, 98 , 53 , 8, ILI9341_GREEN);
  tft.setCursor(150, 267);
  tft.setTextSize(3);
  tft.print("NEXT");

}

void drawPage3Cm() {
  tft.setCursor(20, 10);
  tft.setTextSize(2);
  tft.println("Enter length of      wire in cm");
  tft.println();
  tft.println("  1 cm increments,        25 cm max");

  tft.setCursor(40, 145);
  tft.setTextSize(4);
  tft.print(wireLength);

  tft.drawRoundRect(155,  110,  70,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(156,  111, 68 , 53 , 8, ILI9341_CYAN);
  tft.setCursor(180, 125);
  tft.setTextSize(4);
  tft.print("+");

  tft.drawRoundRect(155,  175,  70,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(156,  176, 68 , 53 , 8, ILI9341_CYAN);
  tft.setCursor(180, 190);
  tft.setTextSize(4);
  tft.print("-");

  tft.drawRoundRect(135,  250,  100,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(136,  251, 98 , 53 , 8, ILI9341_GREEN);
  tft.setCursor(150, 267);
  tft.setTextSize(3);
  tft.print("NEXT");

}

void drawPage4() { 
  tft.setCursor(20, 25);
  tft.setTextSize(2);
  tft.println("Enter number of      wires to cut");

  tft.setCursor(40, 145);
  tft.setTextSize(4);
  tft.print(wireCount);

  tft.drawRoundRect(155,  110,  70,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(156,  111, 68 , 53 , 8, ILI9341_CYAN);
  tft.setCursor(180, 125);
  tft.setTextSize(4);
  tft.print("+");

  tft.drawRoundRect(155,  175,  70,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(156,  176, 68 , 53 , 8, ILI9341_CYAN);
  tft.setCursor(180, 190);
  tft.setTextSize(4);
  tft.print("-");

  tft.drawRoundRect(100,  250,  135,  55,  8, ILI9341_WHITE);
  tft.fillRoundRect(101,  251, 133 , 53 , 8, ILI9341_RED);
  tft.setCursor(115, 267);
  tft.setTextSize(3);
  tft.print("START!");

}

void drawPage5() {
  tft.setCursor(20, 150);
  tft.setTextSize(3);
  tft.print("Dispensing..");
}

void cutWire (int wireL, int wireC, int unit) {

  if (unit == 0) {
    steps = wireL * 200;
  }
  else if (unit == 1) {
    steps = wireL * 80;
  }
  for (int k = 0; k < wireC; k++) {
    digitalWrite(dirPinE, LOW);

    for (int x = 0; x < steps; x++) {
      digitalWrite(stepPinE, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPinE, LOW);
      delayMicroseconds(1000);
    }
    delay(200);

    digitalWrite(dirPinC, LOW);
    for (int i = 0; i < 300; i++) {
      digitalWrite(stepPinC, HIGH);
      delayMicroseconds(1500);
      digitalWrite(stepPinC, LOW);
      delayMicroseconds(1500);
    }
    delay(100);
    digitalWrite(dirPinC, HIGH);
    for (int j = 0; j < 300; j++) {
      digitalWrite(stepPinC, HIGH);
      delayMicroseconds(1500);
      digitalWrite(stepPinC, LOW);
      delayMicroseconds(1500);
    }
    delay(300);
  }
  tft.fillScreen(ILI9341_BLACK);
  drawPage2();
}
void runout(){
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(20,25);
  tft.setTextSize(3);
  tft.setTextColor(ILI9341_RED);
  tft.print("WIRE RUNOUT");
  tft.setCursor(5, 180);
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.print("Feed wire and reset");
}
