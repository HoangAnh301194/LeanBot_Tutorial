#include <Leanbot.h>

void setup() {
  //LbGripper.begin();
  LbGripper.begin(1000 * 4);
}

void loop() {
  LbGripper.shake(4, 15*2, 2);
  delay(1000);

  LbGripper.close(90 + 10);
  delay(1000);

  LbGripper.shake(5, 1*2, 2);
  delay(1000);

  LbGripper.open();
  delay(2000);
}
