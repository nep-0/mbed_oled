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

#include "mbed.h"

class ShabbySPI {
    public:
    /** Create a SPI master connected to the specified pins.
     *
     *  @param mosiPin SPI Master Out, Slave In pin.
     *  @param sclkPin SPI Clock pin.
     *  @param bitsInt SPI frame size.
     *  @param modeInt SPI mode.
     *
     * @code
     * mode | POL PHA
     * -----+--------
     *   0  |  0   0
     *   1  |  0   1
     *   2  |  1   0
     *   3  |  1   1
     * @endcode
     */
    ShabbySPI(PinName mosiPin, PinName sclkPin, int bitsInt, int modeInt);

    /** Write to the SPI Slave and return the response.
     *
     *  @param value Data to be sent to the SPI slave.
     */
    void write(int value);

private:
DigitalOut *mosi;
DigitalOut *sclk;
int bits;
int mode;
};