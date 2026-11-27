#include <Leanbot.h>                    
#include <Arduino_APDS9960.h>

class cLbColorDetector {
  public:
    void detect();
    void printRGB();

  private:
    int objRed, objGreen, objBlue;
};

void cLbColorDetector::detect() {
  int originalBrightness = LbRGB.getBrightness();
  LbRGB.setBrightness(255);

  int rr, gg, bb;

  LbRGB.fillColor(0xFF00FF);
  LbRGB.show();

  while (APDS.colorAvailable() == 0);
  APDS.readColor(rr, gg, bb);
  int r1 = rr;
  int b1 = bb;

  LbRGB.fillColor(0x00FF00);
  LbRGB.show();

  while (APDS.colorAvailable() == 0);
  APDS.readColor(rr, gg, bb);
  int g1 = gg;

  LbRGB.fillColor(0x000000);
  LbRGB.show();

  while (APDS.colorAvailable() == 0);
  APDS.readColor(rr, gg, bb);

  objRed   = (r1 - rr) * 2;
  objGreen = (g1 - gg) * 2;
  objBlue  = (b1 - bb) * 1;

  LbRGB.setBrightness(originalBrightness);
}

void cLbColorDetector::printRGB() {
  Serial.print("RGB: ");
  Serial.print(objRed);
  Serial.print(" ");
  Serial.print(objGreen);
  Serial.print(" ");
  Serial.print(objBlue);
  Serial.println();
}

cLbColorDetector LbColorDetector;

void setup() {
  Leanbot.begin();  // Initialize Leanbot
  if (APDS.begin()) {
    Serial.println("Init APDS-9960 ok.");
  } else {
    Serial.println("Init APDS-9960 error.");
    while (1);  // Stop the program if an error occurs
  }
}

void loop() {
  int distanceMm = Leanbot.pingMm();  // Read distance from HC-SR04 sensor
  if (distanceMm <= 50) {  // If an object is within range

    LbColorDetector.detect();
    LbColorDetector.printRGB();

    LbDelay(2000);
  }
  LbDelay(200);
}
