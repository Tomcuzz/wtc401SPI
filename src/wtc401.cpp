#include "wtc401.h"
#include <SPI.h>

// 8 ms interval. Datasheet specifies minimum 4ms between reads.
#define READ_PERIOD 8
// MAX and MIN are actually 54 and 1, but it is set to 53 and 2 to have better usage
#define SLIDER_MAX 53
#define SLIDER_MIN 2

#define KEY_0_CODE 0x00
#define KEY_1_CODE 0x01
#define KEY_2_CODE 0x02
#define KEY_3_CODE 0x03

WTC401::WTC401(int8_t SCK, int8_t MOSI, int8_t MISO, int8_t CS) {
  this->SCK = SCK;
  this->CS = CS;
  this->MOSI = MOSI;
  this->MISO = MISO;
  this->onKeyEvent = NULL;
  this->lastRead = millis();
  this->lastSliderValue = 0;
  this->lastKey0 = LOW;
  this->lastKey1 = LOW;
  this->lastKey2 = LOW;
  this->lastKey3 = LOW;
}

uint8_t WTC401::read() {
  digitalWrite(CS, LOW);
  uint8_t data = SPI.transfer(0xFF); // Always transfer full 1 bits
  digitalWrite(CS, HIGH);

  return data;
}

void WTC401::begin() {
  pinMode(SCK, OUTPUT);
  pinMode(CS, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);

  digitalWrite(CS, HIGH);

  SPI.pins(SCK, MISO, MOSI, CS);
  SPI.begin();
  lastRead = millis();
}

void WTC401::setOnEvent(std::function<void(KeyEvent, uint8_t)> onKeyEvent) {
  this->onKeyEvent = onKeyEvent;
}

void WTC401::resetButtons() {
  uint8_t L = lastKey0, P = lastKey1, R = lastKey2, S = lastKey3;

  lastKey0 = LOW;
  lastKey1 = LOW;
  lastKey2 = LOW;
  lastKey3 = LOW;

  if (L && onKeyEvent != NULL) {
    onKeyEvent(Key0, LOW);
  }
  if (P && onKeyEvent != NULL) {
    onKeyEvent(Key1, LOW);
  }
  if (R && onKeyEvent != NULL) {
    onKeyEvent(Key2, LOW);
  }
  if (S && onKeyEvent != NULL) {
    onKeyEvent(Key3, LOW);
  }
}

void WTC401::process(uint8_t data) {
  uint8_t flag = data & 0x80;
  data &= 0x7F;

  if (data == 0x7F) { // No Data
    // Reset all buttons
    resetButtons();
    return;
  }

  if (flag) { // Touch Keys
    processButtons(data);
  } else {    // Slider
    processSlider(data);
  }
}

void WTC401::processSlider(uint8_t slider) {
  // Convert to percent
  uint16_t s = (uint16_t)slider - SLIDER_MIN;
  if (s > SLIDER_MAX) { // Negative
    s = 0;
  }
  s *= 100;
  s /= SLIDER_MAX;
  slider = (uint8_t)s;

  if (lastSliderValue != slider) {
    if (onKeyEvent != NULL) {
      onKeyEvent(Slider, slider);
    }
  }

  lastSliderValue = slider;
}

void WTC401::processButtons(uint8_t buttons) {
  switch (buttons) {
    case KEY_0_CODE:
      if (!lastKey0) {
        if (onKeyEvent != NULL) {
          onKeyEvent(KeyRight, HIGH);
        }
      }
      lastKey0 = HIGH;
      break;
    case KEY_1_CODE:
      if (!lastKey1) {
        if (onKeyEvent != NULL) {
          onKeyEvent(KeyPower, HIGH);
        }
      }
      lastKey1 = HIGH;
      break;
    case KEY_2_CODE:
      if (!lastKey2) {
        if (onKeyEvent != NULL) {
          onKeyEvent(KeyLeft, HIGH);
        }
      }
      lastKey2 = HIGH;
      break;
    case KEY_3_CODE:
      if (!lastKey3) {
        if (onKeyEvent != NULL) {
          onKeyEvent(KeyLeft, HIGH);
        }
      }
      lastKey3 = HIGH;
      break;
  }
}


void WTC401::loop() {
  if (millis() - lastRead > READ_PERIOD) {
    uint8_t data = read();
    process(data);
    lastRead = millis();
  }

  if (millis() < lastRead) {
    // Overflow
    lastRead = millis();
  }
}
