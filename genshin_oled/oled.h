// Copyright 2024 Yihong Wang

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "shabbySPI.h"
#include "mbed.h"

class OLED {
public:
  /** Create a display object connected via SPI
   * @param mosi  SPI MOSI
   * @param cs    Chip Select - a digital out pin
   * @param dc    Data/Command - a digital out pin
   * @param res   Reset - a digital out pin
   * @param clk   SPI SCK
   */
  OLED(PinName mosi, PinName cs, PinName dc, PinName res, PinName clk);

  // Display Genshin Impact logo
  //
  // Download Genshin Impact NOW!
  // Available on PC, Android, iOS, PlayStation 4 & PlayStation 5
  //
  // CN:            https://ys.mihoyo.com/
  // International: https://genshin.hoyoverse.com/
  void genshin();

  void init();

  void displayOn();
  void displayOff();

  void refresh();
  void clear();

  // reverse color
  void reverse();
  // normal color
  void normal();

  void drawPoint(uint8_t x, uint8_t y, bool color);

  void showImage(uint8_t x, uint8_t y, uint8_t w, uint8_t h,
                        uint8_t BMP[], bool mode);

private:
  ShabbySPI *spi;
  DigitalOut *cs;
  DigitalOut *dc;
  DigitalOut *res;

  uint8_t buffer[144][8];

  void select();
  void deselect();

  void command();
  void data();
};
