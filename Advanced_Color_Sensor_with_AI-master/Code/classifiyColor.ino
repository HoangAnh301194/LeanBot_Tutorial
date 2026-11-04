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

String classifyColor(int objRed, int objGreen, int objBlue) {
    // Original: --- objBlue <= 8.50  | Integer: if (objBlue <= 8)
    if (objBlue <= 8) {
        // Original: |--- objGreen <= 1.50 | Integer: if (objGreen <= 1)
        if (objGreen <= 1) {
            // |   |--- class: RED
            return "RED";
        }
        // Original: |--- objGreen > 1.50 | Integer: else (objGreen >= 2)
        else {
            // Original: |   |--- objRed <= 7.50 | Integer: if (objRed <= 7)
            if (objRed <= 7) {
                // Original: |   |   |--- objBlue <= 1.50 | Integer: if (objBlue <= 1)
                if (objBlue <= 1) {
                    // Original: |   |   |   |--- objRed <= 2.50 | Integer: if (objRed <= 2)
                    if (objRed <= 2) {
                        // |   |   |   |   |--- class: GREEN
                        return "GREEN";
                    }
                    // Original: |   |   |   |--- objRed > 2.50 | Integer: else (objRed >= 3)
                    else {
                        // |   |   |   |   |--- class: YELLOW
                        return "YELLOW";
                    }
                }
                // Original: |   |   |--- objBlue > 1.50 | Integer: else (objBlue >= 2)
                else {
                    // Original: |   |   |   |--- objGreen <= 4.00 | Integer: if (objGreen <= 4)
                    if (objGreen <= 4) {
                        // |   |   |   |   |--- class: PURPLE
                        return "PURPLE";
                    }
                    // Original: |   |   |   |--- objGreen > 4.00 | Integer: else (objGreen >= 5)
                    else {
                        // |   |   |   |   |--- class: GREEN
                        return "GREEN";
                    }
                }
            }
            // Original: |   |--- objRed > 7.50 | Integer: else (objRed >= 8)
            else {
                // Original: |   |   |--- objBlue <= 5.50 | Integer: if (objBlue <= 5)
                if (objBlue <= 5) {
                    // Original: |   |   |   |--- objGreen <= 6.00 | Integer: if (objGreen <= 6)
                    if (objGreen <= 6) {
                        // |   |   |   |   |--- class: RED
                        return "RED";
                    }
                    // Original: |   |   |   |--- objGreen > 6.00 | Integer: else (objGreen >= 7)
                    else {
                        // |   |   |   |   |--- class: YELLOW
                        return "YELLOW";
                    }
                }
                // Original: |   |   |--- objBlue > 5.50 | Integer: else (objBlue >= 6)
                else {
                    // Original: |   |   |   |--- objGreen <= 43.00 | Integer: if (objGreen <= 43)
                    if (objGreen <= 43) {
                        // |   |   |   |   |--- class: GREEN
                        return "GREEN";
                    }
                    // Original: |   |   |   |--- objGreen > 43.00 | Integer: else (objGreen >= 44)
                    else {
                        // |   |   |   |   |--- class: YELLOW
                        return "YELLOW";
                    }
                }
            }
        }
    }
    // Original: --- objBlue > 8.50 | Integer: else (objBlue >= 9)
    else {
        // Original: |--- objRed <= 2.50 | Integer: if (objRed <= 2)
        if (objRed <= 2) {
            // Original: |   |--- objGreen <= 5.00 | Integer: if (objGreen <= 5)
            if (objGreen <= 5) {
                // Original: |   |   |--- objBlue <= 9.50 | Integer: if (objBlue <= 9)
                if (objBlue <= 9) {
                    // |   |   |   |--- class: BLUE
                    return "BLUE";
                }
                // Original: |   |   |--- objBlue > 9.50 | Integer: else (objBlue >= 10)
                else {
                    // |   |   |   |--- class: PURPLE
                    return "PURPLE";
                }
            }
            // Original: |   |--- objGreen > 5.00 | Integer: else (objGreen >= 6)
            else {
                // |   |   |--- class: BLUE
                return "BLUE";
            }
        }
        // Original: |--- objRed > 2.50 | Integer: else (objRed >= 3)
        else {
            // Original: |   |--- objGreen <= 17.00 | Integer: if (objGreen <= 17)
            if (objGreen <= 17) {
                // Original: |   |   |--- objGreen <= 8.50 | Integer: if (objGreen <= 8)
                if (objGreen <= 8) {
                    // |   |   |   |--- class: PURPLE
                    return "PURPLE";
                }
                // Original: |   |   |--- objGreen > 8.50 | Integer: else (objGreen >= 9)
                else {
                    // Original: |   |   |   |--- objRed <= 4.50 | Integer: if (objRed <= 4)
                    if (objRed <= 4) {
                        // |   |   |   |   |--- class: BLUE
                        return "BLUE";
                    }
                    // Original: |   |   |   |--- objRed > 4.50 | Integer: else (objRed >= 5)
                    else {
                        // |   |   |   |   |--- class: PURPLE
                        return "PURPLE";
                    }
                }
            }
            // Original: |   |--- objGreen > 17.00 | Integer: else (objGreen >= 18)
            else {
                // |   |   |--- class: BLUE
                return "BLUE";
            }
        }
    }
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
