//  Leanbot Object Color Demo  2025-04-21
 
#include <Leanbot.h>
#include <Arduino_APDS9960.h>
 
 
//==============================================================================
//  Object Color
//==============================================================================
 
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
//  Displays an LED color based on the detected color name (string input)
//==============================================================================

void displayRGBFromString(String color){

    CRGB ledColor = CRGB::Black; // no LED

    if (color == "RED") {
        ledColor = CRGB::Red;
    }
    else if (color == "GREEN") {
        ledColor = CRGB::Green;
    }
    else if (color == "BLUE") {
        ledColor = CRGB::Blue;
    }
    else if (color == "YELLOW") {
        ledColor = CRGB::Yellow;
    }
    else if (color == "PURPLE") {
        ledColor = CRGB::Purple;
    }

    LbRGB.fillColor(ledColor);
    LbRGB.show();
}

//==============================================================================
//  Colored-Object Detection (only Reg, Green, Blue)
//==============================================================================

String classifyColor(float objRed, float objGreen, float objBlue) {
	
	return "RED"; // Example
}
 
//==============================================================================
 
 
void setup() {
  Leanbot.begin();                      // initialize Leanbot
  APDS.begin();
}
 
 
void loop() {
  ObjectColorDemo();
  LbDelay(3000);    // delay 3 seconds between each Demo
}
 
 
void ObjectColorDemo() {
  Obj_readRGB();        // activate Led light and APDS color sensor to read object RGB values
 
  // Print out object RGB values for observation
  Serial.println();
  Serial.print(  "ObjRed   = " );
  Serial.println( ObjRed );
  Serial.print(  "ObjGreen = " );
  Serial.println( ObjGreen );
  Serial.print(  "ObjBlue  = " );
  Serial.println( ObjBlue );

  String color = classifyColor(ObjRed, ObjGreen, ObjBlue);
  Serial.print( "Detect Jenga Block: ");
  Serial.println(color);
  displayRGBFromString(color);
}
