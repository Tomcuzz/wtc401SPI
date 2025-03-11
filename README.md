# WTC401

This library enables reading WTC401SPI based touch panels chips.

To get this working, you should connect your Arduino directly to the touch panel board using this pinout:

The J1 connector pinout:

```
 1 -
 2 -
 3 - OUT_FLAG
 4 - GND
 5 -
 6 - CS
 7 - SCK
 8 - SI (Slave Input)
 9 - SO (Slave Output)
10 - VCC
```

## Example

```cpp
#include "wtc401.h"

#define SCK  D5
#define MISO D6
#define CS0  D8
#define MOSI D7

WTC401 panel(SCK, MOSI, MISO, CS0);

void onKeyEvent(KeyEvent event, uint8_t val) {
  switch (event) {
    case Slider:
      Serial.print("Slider: ");
      Serial.println(val, DEC);
      break;
    case KeyLeft:
      Serial.print("KeyLeft: ");
      if (val) {
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      break;
    case KeyPower:
      Serial.print("KeyPower: ");
      if (val) {
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      break;
    case KeyRight:
      Serial.print("KeyRight: ");
      if (val) {
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("OK");
  panel.setOnEvent(&onKeyEvent);
  panel.begin();
}

void loop() {
  panel.loop();
}
```


## Installation

1. Navigate to the Releases page.
2. Download the latest release.
3. Extract the zip file
4. In the Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library
