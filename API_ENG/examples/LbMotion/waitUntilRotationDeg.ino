#include <Leanbot.h>

void setup() {
  Leanbot.begin();                            // initialize Leanbot
}

void loop() {
  int speed = 1000;                           // Speed of movement

  LbMotion.runLR(+speed, -speed);             // let Leanbot turn right
  LbMotion.setZeroOrigin();
  // Rotate 360 degrees, split into approximately 90 degree angles:
  for (int rotation = 0; rotation <= 360; rotation += 90) {
    LbMotion.waitUntilRotationDeg(rotation);
    Leanbot.tone(440*4, 25);                  // Beep every time it rotates 90 degrees
  }
  LbMotion.stopAndWait();                     // let Leanbot stop slowly

  LbDelay(3000);                              // delay 3s


  LbMotion.runLR(-speed, +speed);             // let Leanbot turn left
  LbMotion.setZeroOrigin();
  // Rotate 360 degrees, split into approximately 90 degree angles:
  for (int rotation = 0; rotation <= 360; rotation += 90) {
    LbMotion.waitUntilRotationDeg(-rotation);
    Leanbot.tone(440*4, 25);
  }
  LbMotion.stopAndWait();

  LbDelay(3000);
}
