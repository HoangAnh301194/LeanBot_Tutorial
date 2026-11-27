
#include <Leanbot.h>

void setup() {
  Leanbot.begin();
}

void loop() {
  Leanbot.DCMotor.setPower(200);          // turn forward
  LbDelay(3000);                          // wait 3 seconds
  Leanbot.DCMotor.setPower(0);            // turn off
  LbDelay(3000);

  Leanbot.DCMotor.setPower(-200);         // turn backward
  LbDelay(3000);
  Leanbot.DCMotor.setPower(0);
  LbDelay(3000);
}
