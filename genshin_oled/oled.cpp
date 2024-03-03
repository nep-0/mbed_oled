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

#include "oled.h"
#include "genshin.h"
#include "mbed.h"

OLED::OLED(PinName mosiPin, PinName csPin, PinName dcPin, PinName resPin, PinName clkPin) {
  spi = new ShabbySPI(mosiPin, clkPin, 8, 3);

  cs = new DigitalOut(csPin);
  dc = new DigitalOut(dcPin);
  res = new DigitalOut(resPin);

  res->write(false);
  ThisThread::sleep_for(200ms);
  res->write(true);
}

void OLED::genshin() {
  showImage(0, 0, 128, 64, genshinImage, true);
  refresh();
}

void OLED::select() { cs->write(false); }
void OLED::deselect() { cs->write(true); }

void OLED::command() {
  select();
  dc->write(true);
}

void OLED::data() {
  select();
  dc->write(false);
}

void OLED::init() {
  command();

  spi->write(0xAE); //--turn off oled panel
  spi->write(0x00); //---set low column address
  spi->write(0x10); //---set high column address
  spi->write(0x40); //--set start line address  Set Mapping RAM Display Start
                    ///Line (0x00~0x3F)
  spi->write(0x81); //--set contrast control register
  spi->write(0xCF); // Set SEG Output Current Brightness
  spi->write(
      0xA1); //--Set SEG/Column Mapping     0xa0 flip horizontally 0xa1 normal
  spi->write(
      0xC8); // Set COM/Row Scan Direction   0xc0 flip vertically 0xc8 normal
  spi->write(0xA6); //--set normal display
  spi->write(0xA8); //--set multiplex ratio(1 to 64)
  spi->write(0x3f); //--1/64 duty
  spi->write(
      0xD3);        //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  spi->write(0x00); //-not offset
  spi->write(0xd5); //--set display clock divide ratio/oscillator frequency
  spi->write(0x80); //--set divide ratio, Set Clock as 100 Frames/Sec
  spi->write(0xD9); //--set pre-charge period
  spi->write(0xF1); // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  spi->write(0xDA); //--set com pins hardware configuration
  spi->write(0x12);
  spi->write(0xDB); //--set vcomh
  spi->write(0x40); // Set VCOM Deselect Level
  spi->write(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
  spi->write(0x02); //
  spi->write(0x8D); //--set Charge Pump enable/disable
  spi->write(0x14); //--set(0x10) disable
  spi->write(0xA4); // Disable Entire Display On (0xa4/0xa5)
  spi->write(0xA6); // Disable Inverse Display On (0xa6/a7)
  clear();
  spi->write(0xAF);
}

void OLED::displayOn() {
  command();
  spi->write(0x8D); // enable the charge pump
  spi->write(0x14); // turn on the charge pump
  spi->write(0xAF); // turn on the screen
}

void OLED::displayOff() {
  command();
  spi->write(0x8D); // enable the charge pump
  spi->write(0x10); // turn off the charge pump
  spi->write(0xAE); // turn off the screen
}

void OLED::refresh() {
  data();
  for (uint8_t i = 0; i < 8; i++) {
    spi->write(0xB0 + i); // line starting address
    spi->write(0x00);     // low column address
    spi->write(0x10);     // high column address
    for (uint8_t j = 0; j < 128; j++)
      spi->write(buffer[j][i]);
  }
}

void OLED::clear() {
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0; j < 128; j++) {
      buffer[j][i] = 0;
    }
  }
}

// reverse color
void OLED::reverse() {
  command();
  spi->write(0xA7);
}

// normal color
void OLED::normal() {
  command();
  spi->write(0xA6);
}

void OLED::drawPoint(uint8_t x, uint8_t y, bool color) {
  uint8_t i, m, n;
  i = y / 8;
  m = y % 8;
  n = 1 << m;
  if (color) {
    buffer[x][i] |= n;
  } else {
    buffer[x][i] = ~buffer[x][i];
    buffer[x][i] |= n;
    buffer[x][i] = ~buffer[x][i];
  }
}

void OLED::showImage(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t BMP[],
                     bool mode) {
  uint16_t j = 0;
  uint8_t i, n, temp, m;
  uint8_t x0 = x, y0 = y;
  h = h / 8 + ((h % 8) ? 1 : 0);
  for (n = 0; n < h; n++) {
    for (i = 0; i < w; i++) {
      temp = BMP[j];
      j++;
      for (m = 0; m < 8; m++) {
        if (temp & 0x01)
          drawPoint(x, y, mode);
        else
          drawPoint(x, y, !mode);
        temp = temp >> 1;
        y++;
      }
      x++;
      if ((x - x0) == w) {
        x = x0;
        y0 = y0 + 8;
      }
      y = y0;
    }
  }
}