#include <avr/io.h>
#include <pins.h>

class INDICATOR {
public:
  INDICATOR(uint8_t _dataPin, uint8_t _latchPin, uint8_t _clockPin) {
    dataPin = _dataPin;
    latchPin = _latchPin;
    clockPin = _clockPin;
    setPin(dataPin);
    setPin(latchPin);
    setPin(clockPin);
  }
private:

  const uint8_t DIGIT[4] = { 0b11111110, 0b11111101, 0b11111011, 0b11110111 },
  DIGITAL[11] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111100, 0b00000111, 0b01111111, 0b01100111 , 0b00000000 };
  const uint16_t K[4] = { 1000, 100, 10, 1 };

  uint8_t digitalToIndicate[4] = { 0, 0, 0, 0 },
  dataPin, latchPin, clockPin;
  uint16_t value = 9990;



  void out(uint8_t byte) {
    for (uint8_t i = 0; i < 8; i++) {
      pinWrite(dataPin, ( byte & 1 ));
      byte >>= 1;
      pinWrite(clockPin, true);
      pinWrite(clockPin, false);
    }
  }

  void convert() {
    bool flag = false;
    uint16_t val = value;
    uint8_t digitalIndex = 0;
    for (uint8_t i = 0; i < 4; i++) {
      digitalIndex = val / K[i];
      if (!flag && digitalIndex == 0) {
        digitalToIndicate[i] = DIGITAL[10];
      } else {
        flag = true;
        digitalToIndicate[i] = DIGITAL[digitalIndex];
        val -= digitalIndex * K[i];
      }
    }
    if (value == 0)
      digitalToIndicate[3] = DIGITAL[0];
  }

  void indicate() {
    static uint8_t iteration = 0;
    pinWrite(latchPin, false);
    out(DIGIT[iteration]);
    out(digitalToIndicate[iteration]);
    pinWrite(latchPin, true);
    if (iteration >= 4) {
      iteration = 0;
      convert();
    } else {
      iteration++;
    }
  }

  void checkValue() {
    if (value > 9999)
      value = 0;
  }

public:

  void show() {
    indicate();
  }

  void setValue(uint16_t val) {
    value = val;
    checkValue();
  }

  void increment() {
    value++;
    checkValue();
  }

  void decrement() {
    value--;
    checkValue();
  }

};
