#include <Leanbot.h>

void setup() {
  Leanbot.begin();                 // initialize Leanbot
}

void loop() {
  LbMotion.runLRrpm(+30, +30);     // let Leanbot go straight
  LbDelay(3000);                   // keep running for 3 seconds

  LbMotion.runLRrpm(-30, -30);     // give Leanbot back
  LbDelay(3000);                   // keep running for 3 seconds
}
