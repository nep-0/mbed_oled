#include "mbed.h"
#include "oled.h"

DigitalOut cs(D0);

int main()
{
  OLED oled(A0, A1, A2, A3, A6);
  oled.init();
  oled.genshin();
}
