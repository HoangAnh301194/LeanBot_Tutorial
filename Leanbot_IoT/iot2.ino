//  AIROC26 Demo 01 - Leanbot #2
// - Wait until IoT attributes Color1–Color3 changes from waiting to detected color (by Leanbot #1)
// - Based on the detected color, fetch Neutralizer at either S1 (if Green) or S3 (if Blue)
// - Then transport the matching Neutralizer blocks to eiither G3, E4, or H2
// - Check IoT attributes TrackingLb1 before enter ConflictZone1 to avoid colliding with Leanbot #1
 
#include <Leanbot.h>
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
 
int speed = 2000;
 
void setup() {
  Leanbot.begin();                      // initialize Leanbot
  LbIRLine.doManualCalibration(TB1A);
  //LbIRLine.setThreshold( 96,  123,  156,  110 ) ;    // or measure first and then edit the threshold in the code
  LbIoT.Wifi.begin( WIFI_SSID, WIFI_PASSWORD );
  LbIoT.Asset.begin( IOT_ASSET_ID );
}
 
 
void loop() {
  LbMission.begin(TB1A + TB1B);                  // Start mission when TB1A + TB1B are pressed
  FromCRLToWaitPoint();
  WaitIoTColor1();
  HandleColor1();
  WaitIoTColor2();
  HandleColor2();
  WaitIoTColor3();
  HandleColor3();
  LbMission.end();  
}
 
void FromCRLToWaitPoint(){
  Forward(speed, 65);                           // Move to the Line
  Rotate(-speed, 61);                           // Turn left 
}
 
void DeliverNeutralizer(){
  Forward(speed, 100);                           // Move into cell E
  LbGripper.open();                              // Release the purple block
  Backward(speed, 100);                          // Move back to Waiting Location 
}
 
void FetchGreen() {
  Rotate(speed, 15); 
  Forward(speed, 75);
  LbGripper.close(50);
  Backward(speed, 75);
  Rotate(speed, 125);                           // Turn Right
}
 
void FetchBlue() {
  Forward(speed, 50);
  LbGripper.close(50);
  Backward(speed, 50);
  Rotate(-speed, 155);                           // Turn Right
}
 
void WaitIoTColor1(){
  while (IoTColor1.read()=="waiting"){
    LbDelay(10);
  }
}
 
void WaitUntil_Leanbot1_outside_ConflictZone1() {
  while (IoTTrackingLb1.read() == "ConflictZone1") {
    LbDelay(10);
  }
}
 
 
void WaitPointToG3(){
  followLineMultipleJunctions(2);
  goToJunction();
  Rotate(-speed, 105);                            // Turn left
}
 
void G3ToWaitPoint(){
  Rotate(-speed, 23);  
  goToJunction();
  Forward(speed, 40);
}
 
void HandleColor1() {
  String detectedColor = IoTColor1.read();
  Serial.println( detectedColor );
 
  if (detectedColor == "Red") {
    return;       // do nothing
  }  
 
  if (detectedColor == "Green") {
    FetchGreen();
  }
 
  if (detectedColor == "Blue") {
    FetchBlue();
  }
 
  if (detectedColor == "Unknown") {
    return;       // do nothing
  }  
  
  WaitUntil_Leanbot1_outside_ConflictZone1();
  WaitPointToG3();
  DeliverNeutralizer();
  G3ToWaitPoint();
}
 
 
void WaitIoTColor2(){
  while (IoTColor2.read()=="waiting"){
    LbDelay(10);
  }
}
 
void WaitPointToE4(){
  followLineMultipleJunctions(2);
  goToJunction();
  Rotate(-speed, 23);                            // Turn left
}
 
void E4ToWaitPoint(){
  Rotate(-speed, 110);  
  goToJunction();
  Forward(speed, 40);
}
 
void HandleColor2() {
  String detectedColor = IoTColor2.read();
  Serial.println( detectedColor );
  
  if (detectedColor == "Red") {
    return;       // do nothing
  }  
 
  if (detectedColor == "Green") {
    FetchGreen();
  }
 
  if (detectedColor == "Blue") {
    FetchBlue();
  }
 
  if (detectedColor == "Unknown") {
    return;       // do nothing
  }  
  
  WaitUntil_Leanbot1_outside_ConflictZone1();
  WaitPointToE4();
  DeliverNeutralizer();
  E4ToWaitPoint();
}
 
void WaitIoTColor3(){
  while (IoTColor3.read()=="waiting"){
    LbDelay(10);
  }
}
 
void WaitPointToH2(){
  followLineMultipleJunctions(2);
  goToJunction();
  Rotate(speed, 23);                            // Turn Right
}
 
void H2ToCRL(){
  Rotate(speed, 55);  
  Forward(speed, 100);   
}
 
void WaitPointToCRL(){
  Rotate(-speed, 55);    
  Forward(speed, 100);  
}
 
void HandleColor3() {
  String detectedColor = IoTColor3.read();
  Serial.println( detectedColor );
 
  if (detectedColor=="Red"){      
    WaitPointToCRL();
    return; 
  }
 
  if (detectedColor == "Green") {
    FetchGreen();
  }
 
  if (detectedColor == "Blue") {
    FetchBlue();
  }
  
  if (detectedColor == "Unknown") {
    WaitPointToCRL();
    return;       // do nothing
  }  
 
  WaitPointToH2();
  DeliverNeutralizer();
  H2ToCRL();
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
  int targetMm = distanceMm + LbMotion.getDistanceMm();  // Calculate target distance
 
  while (LbMotion.getDistanceMm() < targetMm) { // Continue until target distance is reached
    runFollowLine();                             // Run line following logic
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