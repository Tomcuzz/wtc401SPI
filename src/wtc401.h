#ifndef _WTC401_
#define _WTC401_

#include <Arduino.h>
#include <functional>

enum KeyEvent {
  Slider,
  Key0,
  Key1,
  Key2,
  Key3,
};

class WTC401 {
  private:
    std::function<void(KeyEvent, uint8_t)> onKeyEvent;
    int8_t SCK, MOSI, MISO, CS;
    long lastRead;

    uint8_t lastSliderValue;
    uint8_t lastKey0;
    uint8_t lastKey1;
    uint8_t lastKey2;
    uint8_t lastKey3;

    uint8_t read();
    void process(uint8_t);
    void processSlider(uint8_t);
    void processButtons(uint8_t);
    void resetButtons();

  public:
    WTC401(int8_t SCK, int8_t MOSI, int8_t MISO, int8_t CS);

    void begin();
    void setOnEvent(std::function<void(KeyEvent, uint8_t)>);
    void loop();

    inline uint8_t ReadKey0()     { return lastKey0;        }
    inline uint8_t ReadKey1()     { return lastKey1;        }
    inline uint8_t ReadKey2()     { return lastKey2;        }
    inline uint8_t ReadKey3()     { return lastKey3;        }
    inline uint8_t ReadSlider()   { return lastSliderValue; }
};

#endif
