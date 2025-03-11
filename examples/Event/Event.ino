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
    case Key0:
      Serial.print("Key0: ");
      if (val) {
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      break;
    case Key1:
      Serial.print("Key1: ");
      if (val) {
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      break;
    case Key2:
      Serial.print("Key2: ");
      if (val) {
        Serial.println("DOWN");
      } else {
        Serial.println("UP");
      }
      break;
    case Key3:
      Serial.print("Key3: ");
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
