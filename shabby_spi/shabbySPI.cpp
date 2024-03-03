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
#include <cstdio>

ShabbySPI::ShabbySPI(PinName mosiPin, PinName sclkPin, int bitsInt,
                     int modeInt) {
  mosi = new DigitalOut(mosiPin);
  sclk = new DigitalOut(sclkPin);
  bits = bitsInt;
  mode = modeInt;

  mosi->write(false);
  if (mode == 0 || mode == 1) {
    sclk->write(false);
  } else {
    sclk->write(true);
  }
}

void ShabbySPI::write(int value) {
  printf("%#2x\n", value);
  if (mode == 0) {
    for (int i = 0; i < bits; i++) {
      if (value & 0x80) {
        mosi->write(true);
      } else {
        mosi->write(false);
      }
      sclk->write(true);
      wait_us(10000);
      sclk->write(false);
      value <<= 1;
    }
  } else if (mode == 1) {
    for (int i = 0; i < bits; i++) {
      sclk->write(true);
      wait_us(10000);
      if (value & 0x80) {
        mosi->write(true);
      } else {
        mosi->write(false);
      }
      sclk->write(false);
      value <<= 1;
    }
  } else if (mode == 2) {
    for (int i = 0; i < bits; i++) {
      if (value & 0x80) {
        mosi->write(true);
      } else {
        mosi->write(false);
      }
      sclk->write(false);
      wait_us(10000);
      sclk->write(true);
      value <<= 1;
    }
  } else if (mode == 3) {
    for (int i = 0; i < bits; i++) {
      sclk->write(false);
      if (value & 0x80) {
        mosi->write(true);
      } else {
        mosi->write(false);
      }
      wait_us(10000);
      sclk->write(true);
      wait_us(10000);
      value <<= 1;
    }
  }
}