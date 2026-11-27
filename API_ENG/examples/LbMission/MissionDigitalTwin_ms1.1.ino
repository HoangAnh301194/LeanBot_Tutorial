
#include <Leanbot.h>

void setup() {
  Leanbot.begin();                           // initialize Leanbot
}

void loop() {
  LbMission.beginDigitalTwin(F( "ms1.1" ));  // start mission ms1.1

  LbMotion.runLR(+1000, +1000);              // go straight forward with speed 1000 steps/s
  LbMotion.waitDistanceMm(+100);             //   for 100 mm distance

  LbMotion.runLR(-1000, -1000);              // go straight backward with speed 1000 steps/s
  LbMotion.waitDistanceMm(-100);             //   for 100 mm distance

  LbMission.end();                           // stop, finish mission
}
