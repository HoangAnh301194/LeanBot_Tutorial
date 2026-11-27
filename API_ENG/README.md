Leanbot API Reference
---

[Arduino basic reference](https://www.arduino.cc/reference/en/)

[Leanbot](#Leanbot)
- [Leanbot.begin](#Leanbotbegin)
- [LbDelay](#LbDelay)
- [LbMission.begin](#LbMissionbegin)


[Motion ](#Motion)
- [LbMotion.runLR](#LbMotionrunLR)
- [LbMotion.stopAndWait ](#LbMotionstopAndWait)
- [LbMotion.waitDistance ](#LbMotionwaitDistance)
- [LbMotion.waitDistanceMm ](#LbMotionwaitDistanceMm)
- [LbMotion.waitRotation ](#LbMotionwaitRotation)
- [LbMotion.waitRotationDeg ](#LbMotionwaitRotationDeg)
- [LbMotion.getDistance](#LbMotiongetDistance)
- [LbMotion.getDistanceMm](#LbMotiongetDistanceMm)
- [LbMotion.getRotation](#LbMotiongetRotation)
- [LbMotion.getRotationDeg](#LbMotiongetRotationDeg)


[Gripper ](#Gripper)
- [LbGripper.open ](#LbGripperopen)
- [LbGripper.close ](#LbGripperclose)
- [LbGripper.moveTo ](#LbGrippermoveTo)
- [LbGripper.moveToLR ](#LbGrippermoveToLR)
- [LbGripper.readL](#LbGripperreadL)
- [LbGripper.readR](#LbGripperreadR)


[Buzzer ](#Buzzer)
- [Leanbot.tone](#Leanbottone)
- [Leanbot.noTone](#LeanbotnoTone)


[RGB Leds ](#RGB-Leds)
- [LbRGB.show](#LbRGBshow)
- [LbRGB.clear](#LbRGBclear)
- [LbRGB[ledX]](#lbrgbledx)
- [LbRGB.fillColor](#LbRGBfillColor)


[Touch Sensors](#Touch-Sensors)
- [LbTouch.read](#LbTouchread)
- [LbTouch.readBits](#LbTouchreadBits)
- [LbTouch.onPress](#LbTouchonPress)


[Ultrasonic Sensor](#Ultrasonic-Sensor)
- [Leanbot.pingCm](#LeanbotpingCm)
- [Leanbot.pingMm](#LeanbotpingMm)


[IR Sensors](#IR-Sensors)
- [LbIRLine.read](#LbIRLineread)
- [LbIRLine.value](#LbIRLinevalue)
- [LbIRLine.print](#LbIRLineprint)
- [LbIRLine.displayOnRGB](#LbIRLinedisplayOnRGB)
- [LbIRLine.isBlackDetected](#LbIRLineisBlackDetected)
- [LbIRLine.doManualCalibration ](#LbIRLinedoManualCalibration)
- [LbIRArray.read](#LbIRArrayread)


[Laze Sensors](#Laze-Sensors)
- [LbLaze.begin](#LbLazebegin)
- [LbLaze.shoot](#LbLazeshoot)

[Color Detector](#Color-Detector)
- [LbColorDetector.detect](#LbColorDetectordetect)
- [LbColorDetector.printRGB](#LbColorDetectorprintRGB)


[Leanbot IoT](README_LeanbotIoT.md)


---

# Leanbot

## Leanbot.begin

### Description
This function initializes Leanbot

### Syntax
```
Leanbot.begin()
```

### Parameters
None

### Returns
None

### Example
```
#include <Leanbot.h>

void setup() {
  Leanbot.begin();                // initialize Leanbot
}
```

### Notes and Warnings
This function must be called at the beginning in the `setup` function.

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot)
## LbDelay (blocking)

### Description
This function makes the program wait (blocking the program flow) until a specified delay time has elapsed, then process the next line of code.
In the meantime, Leanbot keeps running at current velocities.

### Syntax
```
LbDelay(timeMs)
```

### Parameters
- `timeMs`: the number of milliseconds to wait (1000 milliseconds equals one second)
  - Unit: ms
  - Range: [0, 65535]
  - Allowed data types: `unsigned int`

### Returns
None

### Example
This example makes Leanbot moves forward at speed of 400 for 3 seconds, then stop
```
LbMotion.runLR(400, 400);    // let Leanbot move forward
LbDelay(3000);               // wait for 3 seconds (Leanbot keeps moving forward)
LbMotion.runLR(0, 0);        // stop Leanbot
```

### Notes and Warnings
The maximum delay time is `65,535` milliseconds (≈ 65.5 seconds)

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot)
## LbMission.begin (blocking)

### Description
Leanbot waits (blocking the program flow) for the signal to start the mission by simultaneously touching both `TB1A` and `TB1B` buttons on the front of Leanbot.

When receiving the start signal:
- Leanbot will emit the countdown sound `3` – `2` – `1`
- then starts the mission, do tasks

### Syntax
```
LbMission.begin()
```

### Parameters
None

### Returns
None

### Example
```
LbMission.begin();
```

[🔼 Back to top](#leanbot-api-reference)

---

# Motion (actuator)
![Screenshot](image/Motion.png)

## LbMotion.runLR

### Description
This function sets the speed and direction of rotation for the left and right wheels.
The greater the speed, the faster the wheel rotates.

### Syntax
```
LbMotion.runLR(vL, vR)
```

### Parameters
- `vL`: left wheel velocity
- `vR`: right wheel velocity
  - Unit: steps per second
  - Range: [-2000, +2000]
  - Positive value: rotate forward
  - Negative value: rotate backward
  - Allowed data types: `int`

### Returns
None

### Example
This example makes Leanbot moves forward at speed of 400
```
LbMotion.runLR(400, 400);
```
See more:
- [runLR.ino](examples/LbMotion/runLR.ino)

[🔼 Back to top](#leanbot-api-reference)

---


[[ Leanbot ]](#Leanbot) / [[ Motion (actuator) ]](#Motion-%28actuator%29)
## LbMotion.stopAndWait (blocking)

### Description
This function stops Leanbot and waits (blocking the program flow) until Leanbot has decelerated to a complete stop.

### Syntax
```
LbMotion.stopAndWait()
```

### Parameters
None

### Returns
None

### Example

```
LbMotion.stopAndWait();
```

### Notes and Warnings
The higher the speed the Leanbot is running, the longer it takes to stop. The distance traveled before stopping will also be longer.

[🔼 Back to top](#leanbot-api-reference)

---


[[ Leanbot ]](#Leanbot) / [[ Motion (actuator) ]](#Motion-%28actuator%29)
## LbMotion.waitDistance (blocking)

### Description
The program waits (blocking the program flow) in this function until Leanbot travels the desired distance in steps.
Then process the next line of code.

### Syntax
```
LbMotion.waitDistance(distanceStep)
```

### Parameters
- `distanceStep`: the number of steps to wait for Leanbot to travel
  - Unit: steps
  - Allowed data types: `long`

### Returns
None

### Example

```
LbMotion.runLR(400, 400);       // let Leanbot move forward
LbMotion.waitDistance(1500);    // wait for Leanbot to advance 1500 steps
```
See more:
- [waitDistance.ino](examples/LbMotion/waitDistance.ino)

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Motion (actuator) ]](#Motion-%28actuator%29)
## LbMotion.waitDistanceMm (blocking)

### Description
The program waits (blocking the program flow) until Leanbot has moved by (approximately) distance in millimeters.
Then process the next line of code.

### Syntax
```
LbMotion.waitDistanceMm(distanceMm)
```

### Parameters
- `distanceMm`: the distance in millimeters to travel
  - Unit: mm
  - Allowed data types: `int`

### Returns
None

### Example

```
LbMotion.runLR(400, 400);       // let Leanbot move forward
LbMotion.delayDistanceMm(150);  // wait for Leanbot to advance 150mm = 15cm
```
See more:
- [waitDistanceMm.ino](examples/LbMotion/waitDistanceMm.ino)

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Motion (actuator) ]](#Motion-%28actuator%29)
## LbMotion.waitRotation (blocking)

### Description
The program waits (blocking the program flow) until Leanbot has rotated by (approximately) the desired steps.
Then process the next line of code.

### Syntax
```
LbMotion.waitRotation(rotationStep)
```

### Parameters
- `rotationStep`: the number of differential steps to wait for Leanbot to rotate
  - Unit: steps
  - Allowed data types: `long`

### Returns
None

### Example

```
LbMotion.runLR(+400, -400);      // let Leanbot rotate
LbMotion.waitRotation(1500);     // wait for rotating 1500 steps
```
See more:
- [waitRotation.ino](examples/LbMotion/waitRotation.ino)

### Notes and Warnings
Experiment and rotation adjustment are required to find the step value corresponding to the desired rotation angle
- The step value will be different for each Leanbot, speed and moving surface
- For example: with speed 500 and rotation value is 1750, Leanbot will rotate an angle of approximately 90°

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Motion (actuator) ]](#Motion-%28actuator%29)
## LbMotion.waitRotationDeg (blocking)

### Description
The program waits (blocking the program flow) until Leanbot has rotated by (approximately) the desired degrees.
Then process the next line of code.

### Syntax
```
LbMotion.waitRotationDeg(rotationDeg)
```

### Parameters
- `rotationDeg`: the angle to wait for Leanbot to rotate
  - Unit: degree (angle)
  - Allowed data types: `int`

### Returns
None

### Example
```
LbMotion.runLR(+500, -500);        // let Leanbot rotate right
LbMotion.waitRotationDeg(180);     // wait for Leanbot to rotate approximately 180°
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Motion (actuator) ]](#Motion-%28actuator%29)
## LbMotion.getDistance

### Description
This function returns the distance in number of steps which Leanbot has traveled from origin

### Syntax
```
long distance = LbMotion.getDistance()
```

### Parameters
None

### Returns
The traveled distance in number of steps
- Unit: steps
- Data type: `long`

### Example
```
long distance = LbMotion.getDistance();
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Motion (actuator) ]](#Motion-%28actuator%29)
## LbMotion.getDistanceMm

### Description
This function returns the distance in millimeters which Leanbot has traveled from origin

### Syntax
```
long distance = LbMotion.getDistanceMm()
```

### Parameters
None

### Returns
The traveled distance in millimeters
- Unit: mm
- Data type: `long`

### Example
```
long distanceMm = LbMotion.getDistanceMm();
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Motion (actuator) ]](#Motion-%28actuator%29)
## LbMotion.getRotation

### Description
This function returns the angle in number of steps which Leanbot has rotated from origin

### Syntax
```
long rotation = LbMotion.getRotation()
```

### Parameters
None

### Returns
The rorated angle in number of steps
- Unit: steps
- Data type: `long`

### Example
```
long rotation = LbMotion.getRotation();
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Motion (actuator) ]](#Motion-%28actuator%29)
## LbMotion.getRotationDeg

### Description
This function returns the angle in degrees which Leanbot has rotated from origin

### Syntax
```
long rotationDegree = LbMotion.getRotationDeg()
```

### Parameters
None

### Returns
The rorated angle in degrees
- Unit: degree (angle)
- Data type: `long`

### Example
```
long rotationDegree = LbMotion.getRotationDeg();
```

[🔼 Back to top](#leanbot-api-reference)

---

# Gripper (actuator)
![Screenshot](image/LbGripper.png)

## LbGripper.open (blocking)

### Description
This function moves gripper arms to open position (both arms at 0 degree position - perpendicular to the surface)

### Syntax
```
LbGripper.open()
```

### Parameters
None

### Returns
None

### Example
```
LbGripper.open();
```
See more:
- [GripperOpenClose.ino](examples/LbGripper/GripperOpenClose.ino)

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Gripper (actuator) ]](#Gripper-%28actuator%29)
## LbGripper.close (blocking)

### Description
This function moves gripper arms to close position (both arms at 90 degree position - parallel to the surface)


### Syntax
```
LbGripper.close()
```

### Parameters
None

### Returns
None

### Example
```
LbGripper.close();
```
See more:
- [GripperOpenClose.ino](examples/LbGripper/GripperOpenClose.ino)

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Gripper (actuator) ]](#Gripper-%28actuator%29)
## LbGripper.moveTo (blocking)

### Description
This function moves both gripper arms to the same desired angle.

### Syntax
```
LbGripper.moveTo(toAngle)
```

### Parameters
- `toAngle`: the degree to move to
  - Unit: degree (angle)
  - Range: [-30, +120]
  - Allowed data types: `int`

### Returns
None

### Example
The example moves both the gripper arms to the position 45°
```
LbGripper.moveTo(45);
```
See more:
- [GripperMoveTo.ino](examples/LbGripper/GripperMoveTo.ino)

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Gripper (actuator) ]](#Gripper-%28actuator%29)
## LbGripper.moveToLR (blocking)

### Description
This function moves both gripper arms to their desired angles for a desired amount of time (blocking the program flow).

### Syntax
```
LbGripper.moveToLR(toAngleL, toAngleR, timeMs)
```

### Parameters
- `toAngleL`: the degree for left gripper arm to move to
- `toAngleR`: the degree for right gripper arm to move to
  - Unit: degree (angle)
  - Range: [-30, +120]
  - Allowed data types: `int`

- `timeMs`: the time in milliseconds to move both grippers to the target angles
  - Allowed data types: `int`

### Returns
None

### Example
The example moves the left gripper to position 30° and the right gripper to position 60° for 1.5 seconds
```
LbGripper.moveToLR(30, 60, 1500);
```
See more:
- [GripperMoveToLR.ino](examples/LbGripper/GripperMoveToLR.ino)

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Gripper (actuator) ]](#Gripper-%28actuator%29)
## LbGripper.readL

### Description
This function returns the current angle (in degrees) of the left gripper arm.

### Syntax
```
LbGripper.readL()
```

### Parameters
None

### Returns
The current angle (in degrees) of the left gripper arm
- Unit: degree (angle)
- Range: [-30, +120]
- Data type: `int`

### Example
```
int angleL = LbGripper.readL();
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Gripper (actuator) ]](#Gripper-%28actuator%29)
## LbGripper.readR

### Description
This function returns the current angle (in degrees) of the right gripper arm.

### Syntax
```
LbGripper.readR()
```

### Parameters
None

### Returns
The current angle (in degrees) of the right gripper arm
- Unit: degree (angle)
- Range: [-30, +120]
- Data type: `int`

### Example
```
int angleR = LbGripper.readR();
```

[🔼 Back to top](#leanbot-api-reference)

---

# Buzzer (actuator)

## Leanbot.tone

### Description
This function plays sounds with the specified frequency in a duration of time.
- While playing the sound, Leanbot continues to process the next line of code
- The sound will automatically stop after the duration, or call the [Leanbot.noTone](#Leanbot.noTone) function

### Syntax
```
Leanbot.tone(frequency, duration)
```

### Parameters
- frequency: the frequency of the tone in Herzt (Hz)
  - Unit: Herzt (Hz)
  - Allowed data types: `unsigned int`

- duration: (optional) the duration of the tone in milliseconds
  - Unit: ms
  - Allowed data types: `unsigned int`

### Returns
None

### Example
Play sound with frequency 1000 Hz for 1.5 s
```
Leanbot.tone(1000, 1500);
```
See more:
- [tone.ino](examples/Buzzer/tone.ino)
- [toneDuration.ino](examples/Buzzer/toneDuration.ino)

### Notes and Warnings
This function is non-blocking, which means that even if you provide the duration parameter
the sketch execution will continue immediately even if the tone hasn't finished playing.

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Buzzer (actuator) ]](#Buzzer-%28actuator%29)
## Leanbot.noTone

### Description
This function stops the sound played by [Leanbot.tone](#Leanbot.tone)

### Syntax
```
Leanbot.noTone()
```

### Parameters
None

### Returns
None

### Example
```
Leanbot.noTone();
```
See more:
- [noTone.ino](examples/Buzzer/noTone.ino)

[🔼 Back to top](#leanbot-api-reference)

---

# RGB Leds (actuator)
![Screenshot](image/LbRGB.png)
- Leanbot has 7 RGB Leds: `ledA`, `ledB`, `ledC`, `ledD`, `ledE`, `ledF` and `ledO`

## RGB color code
There are 3 different syntax to represent a RGB color:
1. Color name: `CRGB::ColorName`
  - Example: `CRGB::Red`, `CRGB::Green`, `CRGB::Blue`
  - See more: [https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list](https://github.com/FastLED/FastLED/wiki/Pixel-reference#predefined-colors-list)

2. Decimal code: `CRGB(red, green, blue)`
  - Each parameter (red, green, and blue) defines the intensity of the color with a value between 0 and 255
  - Example:
    - `CRGB(255, 0, 0)` is red, because red is set to highest value (255), and the other two (green and blue) are set to 0
    - `CRGB(0, 255, 0)` is green, because green is set to highest value (255), and the other two (red and blue) are set to 0
    - To display black, set all parameters to 0: `CRGB(0, 0, 0)`
    - To display white, set all parameters to 255: `CRGB(255, 255, 255)`
  - See more: [https://www.w3schools.com/colors/colors_picker.asp](https://www.w3schools.com/colors/colors_picker.asp)

3. Hex code: `0xRRGGBB`
  - Concatenate the 3 hex values of the red, green and blue together
  - Example: `0xFF0000` (red), `0x00FF00` (green), `0x0000FF` (blue)

## LbRGB.show

### Description
This function shows all Leds to diplay.

### Syntax
```
LbRGB.show()
```

### Parameters
None

### Returns
None

### Example
```
LbRGB.show();
```

### Notes and Warnings
This function must be called after updating the color of the Leds.

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ RGB Leds (actuator) ]](#RGB-Leds-%28actuator%29)
## LbRGB.clear

### Description
This function clears all Leds to black.

### Syntax
```
LbRGB.clear()
```

### Parameters
None

### Returns
None

### Example
```
LbRGB.clear();
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ RGB Leds (actuator) ]](#RGB-Leds-%28actuator%29)
## LbRGB[ ]

### Description
This function sets a led to the given RGB color.

### Syntax
```
LbRGB[ledX] = color
```

### Parameters
- `ledX`: the led to be set
- `color`: the [RGB color code](#RGB-color-code)

### Returns
None

### Example
```
LbRGB[ledA] = CRGB::Red;              // set `ledA` to red
LbRGB[ledO] = CRGB(0, 255, 0);        // set `ledO` to green
LbRGB[ledD] = 0x0000FF;               // set `ledD` to blue
LbRGB.show();                         // show all Leds to diplay
```
See more:
- [setColor.ino](examples/LbRGB/setColor.ino)

### Notes and Warnings
This function only updates the color value of a led
- You have to call [LbRGB.show](#LbRGB.show) to make the leds actually show the new colors

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ RGB Leds (actuator) ]](#RGB-Leds-%28actuator%29)
## LbRGB.fillColor

### Description
This function fills a shape (set of Leds) with a given RGB color.

### Syntax
```
LbRGB.fillColor(color, shape)
```

### Parameters
- `color`: the [RGB color code](#RGB-color-code)

- `shape`: a set of Leds to be set
  - Allowed data types: `byte`

### Returns
None

### Example
The example sets the 4 Leds A, B, C and D (a smiley shape) to green
```
byte LbSmiley = BITMAP(ledA, ledB, ledC, ledD);   // create smiley shape
LbRGB.fillColor(CRGB::Green, LbSmiley);           // fill green color
LbRGB.show();                                     // show all Leds to diplay
```
See more:
- [fillColor.ino](examples/LbRGB/fillColor.ino)

[🔼 Back to top](#leanbot-api-reference)

---

# Touch Sensors
![Screenshot](image/LbTouch.png)
- Leanbot has 4 touch sensors: `TB1A`, `TB1B`, `TB2A` and `TB2B`

## LbTouch.read

### Description
This function reads the state of the specified touch sensors.

### Syntax
```
value = LbTouch.read(tbX)
```

### Parameters
`tbX`: the touch sensor to read. Valid choices are:
- TB1A
- TB1B
- TB2A
- TB2B

### Returns
The state the touch sensor
- Value `0`: the sensor is being released
- Value `1`: the sensor is being touched
- Data type: `byte`

### Example
The example reads the state of the sensors `TB1A` and `TB2A`
```
byte value1A = LbTouch.read(TB1A);
byte value2A = LbTouch.read(TB2A);
```
See more:
- [ledControl.ino](examples/LbTouch/ledControl.ino)
- [wheelControl.ino](examples/LbTouch/wheelControl.ino)

### Notes and Warnings
Multiple sensors can be combined and read at once time.
```
LbTouch.read(TB1A | TB1B)
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Touch Sensors ]](#Touch-Sensors)
## LbTouch.readBits

### Description
This function reads the state of all 4 touch sensors.

### Syntax
```
touchBits = LbTouch.readBits()
```

### Parameters
- None

### Returns
The binary state of 4 touch sensors
- Value `0`: the sensor is being released
- Value `1`: the sensor is being touched
- Data type: `byte`

### Example
```
byte touchBits = LbTouch.readBits();
```
See more:
- [readBits.ino](examples/LbTouch/readBits.ino)

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Touch Sensors ]](#Touch-Sensors)
## LbTouch.onPress

### Description
This function reads the touching event of the specified touch sensors.

### Syntax
```
isOnPress = LbTouch.onPress(tbX)
```

### Parameters
- `tbX`: the touch sensor to read

### Returns
The touching event the touch sensor
- Value `true`: the sensor has NOT just been touched, either released or being held
- Value `false`: the sensor has just been touched
- Data type: `bool`

### Example
The example reads the touching state of the sensor `TB1A`
```
bool value1A = LbTouch.onPress(TB1A);
```
See more:
- [onPress.ino](examples/LbTouch/onPress.ino)

### Notes and Warnings
Multiple sensors can be combined and read at once time.
```
LbTouch.onPress(TB1A | TB1B)
```

[🔼 Back to top](#leanbot-api-reference)

---

# Ultrasonic Sensor

### Notes and Warnings
The ultrasonic sensor can be triggered as fast as every 50 ms, or 20 times each second
- You should wait 50 ms before the next ping
- This is to ensure the ultrasonic __beep__ has faded away and will not cause a false echo on the next ranging

## Leanbot.pingCm

### Description
This function sends a ping and returns the front distance measured in centimeters.

### Syntax
```
distanceCm = Leanbot.pingCm()
```

### Parameters
None

### Returns
The front distance measured in centimeters
- Unit: cm
- Data type: `unsigned int`

### Example
```
unsigned int distanceCm = Leanbot.pingCm();
```
See more:
- [pingCm.ino](examples/Ping/pingCm.ino)

### Notes and Warnings
The maximum sensor distance is 300 cm, outside this distance, the function will return 1,000 cm.

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ Ultrasonic Sensor ]](#Ultrasonic-Sensor)
## Leanbot.pingMm

### Description
This function sends a ping and returns the front distance measured in millimeters.

### Syntax
```
distanceMm = Leanbot.pingMm()
```

### Parameters
None

### Returns
The front distance measured in centimeters
- Unit: mm
- Data type: `unsigned int`

### Example
```
unsigned int distanceMm = Leanbot.pingMm();
```

### Notes and Warnings
The maximum sensor distance is `3,000 mm`, outside this distance, the function will return `10,000 mm`

[🔼 Back to top](#leanbot-api-reference)

---

# IR Sensors

![Screenshot](image/LbIRArray.png)
- Leanbot has 8 IR sensors (order 0 - 7), for different purposes:

| Function             | Sensors                   |
| -------------------- |:-------------------------:|
| Line detection       | ir3R - ir1R - ir0L - ir2L |
| Table edge detection | ir5R - ir4L               |
| Obstacles detection  | ir7R - ir6L               |

## LbIRLine.read

### Description
This function reads the value of 4 bar sensors. Used to check the position of the black line relative to Leanbot.

### Syntax
```
lineState = LbIRLine.read()
```

### Parameters
None

### Returns
The binary state of 4 bar sensors
- Value `0`: the sensor is on the white surface
- Value `1`: the sensor is on the black line
- Data type: `byte`

### Example
```
byte lineState = LbIRLine.read();
```
See more:
- [readLineState.ino](examples/LbIRLine/readLineState.ino)
- [followLine.ino](examples/LbIRLine/followLine.ino)

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ IR Sensors ]](#IR-Sensors)
## LbIRLine.value

### Description
This function returns the value of 4 bar sensors which are read before.

### Syntax
```
LbIRLine.value()
```

### Parameters
None

### Returns
The 4 line sensors value
- Data type: `byte`

### Example
```
byte lineValue = LbIRLine.value();
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ IR Sensors ]](#IR-Sensors)
## LbIRLine.print

### Description
This function sends the value of the 4 bar sensors (which are read before) to the computer.

### Syntax
```
LbIRLine.print()
```

### Parameters
None

### Returns
None

### Example
```
LbIRLine.print();
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ IR Sensors ]](#IR-Sensors)
## LbIRLine.displayOnRGB

### Description
This function displays the 4 bar sensors result on RGB Leds with color.
- If any sensor is on the black line, the corresponding RGB Led will light up

### Syntax
```
LbIRLine.displayOnRGB(color)
```

### Parameters
- `color`: the [RGB color code](#RGB-color-code)

### Returns
None

### Example
```
LbIRLine.read();                     // update line state
LbIRLine.displayOnRGB(CRGB::Blue);   // display result on Leds
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ IR Sensors ]](#IR-Sensors)
## LbIRLine.isBlackDetected

### Description
This function checks whether one of the 4 bar sensors is on the black line.

### Syntax
```
LbIRLine.isBlackDetected()
```

### Parameters
None

### Returns
- `true` : the bars sensor is on the black line
- `false`: the bars sensor is NOT on the black line
- Data type: `bool`

### Example
The example lets Leanbot moving forward until the end of the black line
```
LbMotion.runLR(400, 400);                // move forward
while ( LbIRLine.isBlackDetected() );    // keep moving as long as Leanbot can see the black line
LbMotion.stopAndWait();                  // stop
```

[🔼 Back to top](#leanbot-api-reference)

---

[[ Leanbot ]](#Leanbot) / [[ IR Sensors ]](#IR-Sensors)
## LbIRLine.doManualCalibration (blocking)

### Description
This function does 3-step light level calibration with touch button.

### Syntax
```
LbIRLine.doManualCalibration(tbX)
```

### Parameters
- `tbX`: the touch sensor used to perform black/white sampling
  - Leanbot has 4 touch sensors: `TB1A`, `TB1B`, `TB2A` and `TB2A`

### Returns
None

### Example
```
LbIRLine.doManualCalibration(TB1A);
```
See more:
- [lineCalibration.ino](examples/LbIRLine/lineCalibration.ino)
- [calibrationThenFollowLine.ino](examples/LbIRLine/calibrationThenFollowLine.ino)

### Notes and Warnings
It is recommended to perform this step before starting the mission or when there are changes in ambient light or moving surface.

[🔼 Back to top](#leanbot-api-reference)

---

## LbIRArray.read

### Description
This function reads the value of the specified sensor.
The darker the surface, the higher the sensor value.

### Syntax
```
LbIRArray.read(irX)
```

### Parameters
- `irX`: the sensor to read

### Returns
The sensor value: 0 - 768
- Data type: `int`

### Example
The example reads the value of sensor `ir0L` and sends the result to the computer
```
int value = LbIRArray.read(ir0L);    // read the sensor value
Serial.println(value);               // transfer the results to the computer
```

[🔼 Back to top](#leanbot-api-reference)

---


# Laze Sensors

Before using `Laze Sensors`, declare:

```
class cLbLaze {
  public:
    void begin();
    void shoot();
};

void cLbLaze::begin() {
  pinMode(13, OUTPUT);
}

void cLbLaze::shoot() {
  Serial.println("shoot");
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);
}

cLbLaze LbLaze;
```

## LbLaze.begin()

### Description

The function `LbLaze.begin()` initializes the Leanbot laser sensor and sets the laser control pin as an output (`OUTPUT`).
This function must be called in `setup()` before using `LbLaze.shoot()`.

### Syntax
```
LbLaze.begin();
```

### Parameters
 
None  

### Returns

None  

## LbLaze.shoot()

### Description

The function `LbLaze.shoot()` activates the Leanbot laser sensors, turns on the laser beam for 2 seconds, and then turns it off.

### Syntax
```
LbLaze.shoot()
```

### Parameters

None

### Returns

None

### Example

- [Laze.ino](examples/Laze/Laze.ino)

[🔼 Back to top](#leanbot-api-reference)


# Color Detector

Before using `Color Detector`, declare:

```
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
```

- And in `setup()`, call it to check the sensor:

```
if (APDS.begin()) {
  Serial.println("Init APDS-9960 ok.");
} else {
  Serial.println("Init APDS-9960 error.");
  while (1);
}
```

## LbColorDetector.detect()  

### Description
  
The function `LbColorDetector.detect()` detects the color of an object using the APDS-9960 sensor.
This function changes the LED color to determine the object's RGB values and stores them in an internal variable.  

**Note:** The APDS-9960 sensor must be initialized before calling this function.

The optimal detection distance is less than 5 cm.

### Syntax 
 
```
LbColorDetector.detect();
```

### Parameters

None

### Returns

None


## LbColorDetector.printRGB()

### Description

The function `LbColorDetector.printRGB()` prints the RGB values of the object detected by the color sensor.
These values are calculated in the `LbColorDetector.detect()` function.

### Syntax

```
LbColorDetector.printRGB();
```

### Parameters

None

### Returns

None

### Example

```
LbColorDetector.detect();
LbColorDetector.printRGB();
```

Expected output on the Serial Monitor.

```
RGB: 120 85 60
```

### See more

- [ColorDetector.ino](examples/APDS/ColorDetector.ino)

[🔼 Back to top](#leanbot-api-reference)
