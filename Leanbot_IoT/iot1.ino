//  AIROC26 Demo 01 - Leanbot #1
// - Go to G3, E4, H2, and then back to CRL in order
// - Detect color at each site, update to IoT attributes Color1, Color2, Color3
// - Update Leanbot #1 general position (NoConflict, ConflictZone1) to IoT attribute TrackingLb1
 
#include <Leanbot.h>
#include <Arduino_APDS9960.h>
#include "LeanbotIoT.h"
 
#define  WIFI_SSID      F("CHUYEN-HV")
#define  WIFI_PASSWORD  F("")
 
// Asset - https://iot.pythaverse.space/manager/?realm=pythaverse#/assets/false/42aOLg8W86rX9SCw4F5VWZ
#define  IOT_ASSET_ID   F( "5JeuIta24yYXt2uZMZyukD" )
 
auto IoTColor1 = LbIoT.Asset.attribute(F( "Color1" ));
auto IoTColor2 = LbIoT.Asset.attribute(F( "Color2" ));
auto IoTColor3 = LbIoT.Asset.attribute(F( "Color3" ));
 
auto IoTTrackingLb1 = LbIoT.Asset.attribute(F( "TrackingLb1" ));
 
//==============================================================================
//  Object Color
//==============================================================================
int speed = 2000;
 
int ObjRed, ObjGreen, ObjBlue;                 // measured object RGB color
 
void _coreRead(int& r, int& g, int& b, CRGB ledColor) {  // measure RGB values under ledColor + ambient light condition
  LbRGB.fillColor(ledColor);
  LbRGB.show();
  while ( ! APDS.colorAvailable());
  APDS.readColor(r, g, b);
}
 
void Obj_readRGB() {
  int rRed, gGreen, bBlue;
  int rr, gg, bb;
 
  int originalBrightness = LbRGB.getBrightness();        // remember LED brightness
  LbRGB.setBrightness(255);                              // change to maximum LED brightness
 
  _coreRead( rRed, gg,   bb,  CRGB::Red   );   // rRed:     red value  under   Red LED + ambient light
  _coreRead( rr, gGreen, bb,  CRGB::Lime  );   // gGreen: green value  under Green LED + ambient light - CRGB::Lime is #00FF00 from https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list
  _coreRead( rr,   gg, bBlue, CRGB::Blue  );   // bBlue:   blue value  under  Blue LED + ambient light
  _coreRead( rr,   gg,   bb,  CRGB::Black );   // (ambient) RGB values under    no LED + ambient light
 
  LbRGB.setBrightness(originalBrightness);               // restore LED brightness
 
  ObjRed   = rRed   - rr;    // remove (varying) ambient light
  ObjGreen = gGreen - gg;
  ObjBlue  = bBlue  - bb;
}
 
 
//==============================================================================
//  Basic Colored-Object Detection (only Red, Green, Blue)
//==============================================================================
 
boolean BasicObj_isRed() {
  if ( ObjRed <= ObjGreen )  return false;
  if ( ObjRed <= ObjBlue  )  return false;
  if ( ObjRed <= 3        )  return false;
  return true;
}
 
boolean BasicObj_isGreen() {
  if ( ObjGreen <= ObjRed  )  return false;
  if ( ObjGreen <= ObjBlue )  return false;
  if ( ObjGreen <= 3       )  return false;
  return true;
}
 
boolean BasicObj_isBlue() {
  if ( ObjBlue <= ObjRed   )  return false;
  if ( ObjBlue <= ObjGreen )  return false;
  if ( ObjBlue <= 3        )  return false;
  return true;
}
 
String Obj_detectColor() {
  Obj_readRGB();
 
  if (BasicObj_isRed()) {
    return "Red";
  }
 
  if (BasicObj_isGreen()) {
    return "Green";
  }
 
  if (BasicObj_isBlue()) {
    return "Blue";
  }
  
  return "Unknown";
} 
 
//==============================================================================
 
void setup() {
  Leanbot.begin();                      // initialize Leanbot
  LbIRLine.doManualCalibration(TB1A);
  //LbIRLine.setThreshold( 119,  104,   81,   90 ) ;    // or measure first and then edit the threshold in the code
  APDS.begin();
  LbIoT.Wifi.begin( WIFI_SSID, WIFI_PASSWORD );
  LbIoT.Asset.begin( IOT_ASSET_ID );
  IoTColor1.write("waiting");
  LbDelay(500);
  IoTColor2.write("waiting");
  LbDelay(500); 
  IoTColor3.write("waiting");
  LbDelay(500); 
  IoTTrackingLb1.write("NoConflict");
  LbDelay(500); 
}
 
String detectedColor;
 
void loop() {
  LbMission.begin(TB1A + TB1B);                  // Start mission when TB1A + TB1B are pressed
  FromCRLtoG3();
  detectedColor = Obj_detectColor();
  IoTColor1.write(detectedColor);
  HandleColor(detectedColor);
 
  FromG3toE4();
 
  detectedColor = Obj_detectColor();
  IoTColor2.write(detectedColor);
  HandleColor(detectedColor);
 
  FromE4toH2();
  
  detectedColor = Obj_detectColor();
  IoTColor3.write(detectedColor);
  HandleColor(detectedColor);
 
  FromH2toCRL();
 
  LbMission.end();  
}
 
void FromCRLtoG3(){
  IoTTrackingLb1.write("ConflictZone1");
  Forward(speed, 110);                          // Go  to the line.
  Rotate(-speed, 60);                           // Turn Left
  Forward(speed, 60);                           // Move to the G3
}
 
void FromG3toE4(){
  Backward(speed, 60);                         // Exit G3
  Rotate(speed, 63);                           // Turn Right
  Forward(speed, 60);                          // Go to E4
}
 
void FromE4toH2(){
  Backward(speed, 60); 
  Rotate(speed, 62);                           // Turn Right
  Forward(speed, 60);                          // Go to 
}
 
void FromH2toCRL(){
  Backward(speed, 20); 
  Rotate(speed, 46);                            // Turn Right
  IoTTrackingLb1.write("NoConflict");
  Forward(speed, 230);                          // Go to 
}
 
void HandleColor(String detectedColor){
  if (detectedColor=="Red") {
    Forward(speed, 20);              						 // push Block Red
    Backward(speed, 20); 
  }
}
 
void Forward(int speed, int distanceMm) {
  LbMotion.runLR(speed, speed);                  // Move forward with given speed
  LbMotion.waitDistanceMm(distanceMm);           // Wait until distance is reached
  LbMotion.stopAndWait();                        // Stop and wait
}
 
void Backward(int speed, int distanceMm) {
  LbMotion.runLR(-speed, -speed);                // Move backward with given speed
  LbMotion.waitDistanceMm(distanceMm);           // Wait until distance is reached
  LbMotion.stopAndWait();                        // Stop and wait
}
 
void Rotate(int speed, int deg) {
  LbMotion.runLR(speed, -speed);                 // Rotate robot in place
  LbMotion.waitRotationDeg(deg);                 // Wait until rotation angle is reached
  LbMotion.stopAndWait();                        // Stop and wait
}
 
void followLineDistance(int distanceMm) {
  int targetMm = distanceMm + LbMotion.getDistanceMm(); // Calculate targ et distance
 
  while (LbMotion.getDistanceMm() < targetMm) { 				// Continue until target distance is reached
    runFollowLine();                             				// Run line following logic
  }
}
 
void runFollowLine() {
  //LbRGB.fillColor(CRGB::Black); // Off RGB
  int lineValue = LbIRLine.read();                // Read IR line sensor value
  LbIRLine.displayOnRGB(CRGB::DarkCyan);          // Display sensor status on RGB LED
 
  switch (lineValue) {
 
    case 0b0100:                                 // .o..
      LbMotion.runLR(+speed/2, +speed);          // Adjust slightly to the left
      break;
 
    case 0b0010:                                 // ..o.
      LbMotion.runLR(+speed, +speed/2);          // Adjust slightly to the right
      break;
 
    case 0b1100:                                 // oo..
      LbMotion.runLR(     0, +speed);            // Strong left correction
      break;
 
    case 0b0011:                                 // ..oo
      LbMotion.runLR(+speed, 0);                 // Strong right correction
      break;
 
    case 0b1000:                                 // o...
      LbMotion.runLR(-speed/2, +speed/2);        // Sharp left turn
      break;
 
    case 0b0001:                                 // ...o
      LbMotion.runLR(+speed/2, -speed/2);        // Sharp right turn
      break;
 
    default:
      LbMotion.runLR(+speed, +speed);            // Go straight
      break;
  }
}
 
void followLineUntilJunction() {
  while (LbIRLine.read() != 0b1111) {            // Continue until full junction is detected
    runFollowLine();                             // Run line following logic
  }
}  
 
void goToJunction() {
  followLineUntilJunction();                     // Continue until junction is detected
  followLineDistance(20);                        // Move forward to the purple block
  LbMotion.stopAndWait();                        // Stop and wait
}
 
void followLineMultipleJunctions(int junctionCount) {
  for (int i = 0; i < junctionCount; i++) {
    followLineDistance(30);
    followLineUntilJunction();
  }
}