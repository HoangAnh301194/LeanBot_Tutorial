#include <Leanbot.h>                    // use Leanbot library
#include <Arduino_APDS9960.h>
 
/*--------------------------------------------------------------------------------------------*/
 
const bool EnableCounter = true;
const int POSITION_PER_COLOR   = 20;
const int MEASURE_PER_POSITION =  1;
const int MEASURE_AGAIN_PER_POSITION = 10;
const int AbnormalThreshold = -1;
 
/*--------------------------------------------------------------------------------------------*/
 
#define objColorNum    5
const char* objColor[objColorNum] = {"RED", "YELLOW", "GREEN", "BLUE", "PURPLE"};
const uint32_t objColorRGB[objColorNum] = {0xFF0000, 0xFFFF00, 0x00FF00,0x0000FF, 0xFF00FF};
int objColorIndex = 0;
 
/*--------------------------------------------------------------------------------------------*/
 
void LbRGB_showColor(CRGB ledColor){
  LbRGB.fillColor(ledColor);
  LbRGB.show();
}
 
void changeObjColor(int change){ //  dir = +1 or -1
  objColorIndex += change;
  if (objColorIndex >= objColorNum) objColorIndex = 0;
  if (objColorIndex < 0           ) objColorIndex = objColorNum - 1;
 
  LbRGB_showColor(objColorRGB[objColorIndex]); /// Show objColor RGB
}

/*--------------------------------------------------------------------------------------------*/
 
long getRandomId() {
  static unsigned long seed = 0;
  seed += micros();
  seed += analogRead(A0);
  seed += analogRead(A1);
  seed += analogRead(A2);
  seed += analogRead(A3);
  seed += analogRead(A4);
  seed += analogRead(A5);
  seed += analogRead(A6);
  seed += analogRead(A7);
  randomSeed(seed);
 
  seed = random(100000000, 999999999);
  return seed;
}

/*--------------------------------------------------------------------------------------------*/
 
void printKey(const char* key) { /// "key":
  Serial.print( "\"" );
  Serial.print(key);
  Serial.print( "\":" );
}
 
void printKV(const char* key, int value) { /// "key":value
  printKey(key);
  Serial.print(value);
}
 
void printKV(const char* key, int value, const char separator) { /// "key":value (separator)
  printKV(key, value);
  Serial.println(separator);
}
 
void printKV(const char* key, const char* value, const char separator) { /// "key":value (separator)
  printKey(key);
  Serial.print( '\"' );
  Serial.print(value);
  Serial.print( '\"' );
  Serial.print(separator);
}
 
/*--------------------------------------------------------------------------------------------*/
 
int ObjRed, ObjGreen, ObjBlue;                 // measured object RGB color
 
void _coreRead(int& r, int& g, int& b, CRGB ledColor) {  // measure RGB values under ledColor + ambient light condition
  LbRGB_showColor(ledColor);
  LbDelay(1);
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

void printMeasureResult(void){
  printKV("objRed",ObjRed,',');    /// print ObjRed/Green/Blue
  printKV("objGreen",ObjGreen,',');
  printKV("objBlue",ObjBlue);  
}

void doMeasurement(){ /// do full read RGB sample then print when TB1A was touched
  Serial.println( "{" );
 
  printKV( "objColor", objColor[objColorIndex], ',' );
  Serial.println();
 
  Obj_readRGB(); // Measure object color by comparing RGB values with and without illumination
 
  changeObjColor(0);
 
  printMeasureResult();
 
  getRandomId(); // reset random seed
  Serial.println( "\n}," );
}

/*--------------------------------------------------------------------------------------------*/
 
void randomDelay(uint32_t interval) { 
  LbDelay(random(interval/2, interval));
}

/*--------------------------------------------------------------------------------------------*/
 
void longBeep(void){
  Leanbot.tone(1500, 500);
}
 
void shortBeep(void){
  Leanbot.tone(1500, 50);
}
 
void waitTouchTB2A(){ /// Block until TB2A is pressed
  while(true){
    // also checks TB1A and TB1B
    if (LbTouch.onPress(TB2A)) return;
    if (LbTouch.onPress(TB1A)) changeObjColor(+1);
    if (LbTouch.onPress(TB1B)) changeObjColor(-1);
  }
}
 
/*--------------------------------------------------------------------------------------------*/
 
void setup() {
  Leanbot.begin();                      // initialize Leanbot
  APDS.begin();
}
 
void loop() {
  longBeep();                                          // change color indication
  changeObjColor(+1);                                  // auto change to next color
 
  for (int i = 0; i < POSITION_PER_COLOR; i++) {       // multiple positions for each color
    waitTouchTB2A();
    for (int k = 0; k < MEASURE_PER_POSITION; k++) {   // multiple measurements for each position
      shortBeep();                                     // measurement indication
      doMeasurement();
      randomDelay(100);
    }
  }
}