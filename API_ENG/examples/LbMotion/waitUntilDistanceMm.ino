#include <Leanbot.h>

void setup() {
  Leanbot.begin();                            // initialize Leanbot
}

void loop() {
  int speed = 500;                            // Speed of movement

  LbMotion.runLR(+speed, +speed);             // let Leanbot go straight
  LbMotion.setZeroOrigin();
  for (int dMm = 0; dMm <= 100; dMm += 25) {  // move 100 mm, split into 25 mm . segments
    LbMotion.waitUntilDistanceMm(dMm);        // wait for Leanbot to reach the desired location
    Leanbot.tone(440*4, 25);                  // Beep every 25 mm
  }
  LbMotion.stopAndWait();                     // let Leanbot stop slowly

  LbDelay(3000);                              // delay 3s


  LbMotion.runLR(-speed, -speed);             // let Leanbot go backwards
  LbMotion.setZeroOrigin();
  for (int dMm = 0; dMm <= 100; dMm += 25) {
    LbMotion.waitUntilDistanceMm(-dMm);       // wait for Leanbot to retreat to the desired position
    Leanbot.tone(440*4, 25);
  }
  LbMotion.stopAndWait();

  LbDelay(3000);
}
