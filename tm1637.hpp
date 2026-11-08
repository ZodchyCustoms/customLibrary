#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <customs/pins.h>

/*
Число для отображения хранится в массиве _digitsToShow[4] в виде кодов чисел.
Функция _updateDisplay() отправляет их на экран - каждый байт это цифра (код).
Для включения двоеточия необходимо установить старший бит во всех и отправить байты.
Для выбора разряда необходимо добавить его номер к CMD_DATA.

После запуска показывает четыре тире!
*/


class TM1637 {
public:
  enum class Razr
  {
    SEC = 0,
    MIN = 1
  };

  TM1637(uint8_t dio, uint8_t clk)
  {
    this->_dio = dio;
    this->_clk = clk;
    setPinOutput(_dio);
    setPinOutput(_clk);
    _init();
  }

  // Start animation
  void hello()
  {
    // Anim
    cli();
    uint8_t i = 0, j = 0;
    uint16_t _pause = 70;
    for (; i < 4; i++)
    {
      _digitsToShow[i] |= (1 << j);
      _updateDisplay();
      _delay_ms(_pause);
    }
    j++;
    _digitsToShow[3] |= (1 << j++);
    _updateDisplay();
    _delay_ms(_pause);
    _digitsToShow[3] |= (1 << j++);
    _updateDisplay();
    _delay_ms(_pause);
    for (; i != 255; i--)
    {
      _digitsToShow[i] |= (1 << j);
      _updateDisplay();
      _delay_ms(_pause);
    }
    j++;
    _digitsToShow[0] |= (1 << j++);
    _updateDisplay();
    _delay_ms(_pause);
    _digitsToShow[0] |= (1 << j);
    _updateDisplay();
    _delay_ms(_pause);
    i = 0;
    for (; i < 4; i++)
    {
      _digitsToShow[i] = _chars[1];
    }
    _updateDisplay();
    sei();
  }

  // convert value and show on the display
  void show(uint16_t value)
  {
    _convert(value);
    _updateDisplay();
  }

  // Добавить энум или дефайны чтобы при вызове по человечески писать, а не число
  void showChar(uint8_t numberOfChar)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      _digitsToShow[i] = _chars[numberOfChar];
    }
    _updateDisplay();
  }

  void setBrightness(uint8_t level)
  {
    if (level > 7)
      level = 7;
    if (level == 0)
      _brightness = CMD_OFF;
    else
      _brightness = CMD_BRIGHT + level;
    _start();
    _send(_brightness);
    _stop();
  }

  void setPoint(bool state)
  {
    _pointState = state;
    _updateDisplay();
  }

  // Специально для таймера. Изменить только два старших или младших разряда.
  void showTime(Razr razr, uint8_t value)
  {
    uint8_t i, j;
    switch (razr)
    {
      case Razr::MIN:
      i = 0;
      j = 1;
      break;
      case Razr::SEC:
      i = 2;
      j = 3;
      break;
    }
    _digitsToShow[i] = _codesOfDigits[value / 10];
    _digitsToShow[j] = _codesOfDigits[value % 10];
    _updateDisplay();
  }

private:

  void _start()
  {
    setPin(_dio, true);
    setPin(_clk, true);
    _delay_us(2);
    setPin(_dio, false);
    _delay_us(2);
    setPin(_clk, false);
  }

  void _stop()
  {
    setPin(_dio, false);
    setPin(_clk, false);
    _delay_us(2);
    setPin(_clk, true);
    _delay_us(2);
    setPin(_dio, true);
  }

  void _strobe()
  {
    _delay_us(2);
    setPin(_clk, true);
    _delay_us(2);
    setPin(_clk, false);
    _delay_us(2);
  }

  void _checkPointState(uint8_t p)
  {
    switch (_pointState)
    {
      case true:
      _digitsToShow[p] |= (1 << 7);
      break;
      case false:
      _digitsToShow[p] &= ~(1 << 7);
      break;
    }
  }

  // just send a byte of data and waiting for ACK
  void _send(uint8_t value)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      setPin(_dio, (value & 1));
      _strobe();
      value >>= 1;
    }
    setPinInput(_dio);
    _delay_us(2);
    setPin(_clk, true);
    while(!readPin(_dio)) {}
    setPin(_clk, false);
    setPinOutput(_dio);
  }

  // start -> send -> stop
  void _sendByte(uint8_t data)
  {
    _start();
    _send(data);
    _stop();
  }

  // update display
  void _updateDisplay()
  {
    _start();
    _send(CMD_DATA);
    for (uint8_t i = 0; i < 4; i++)
    {
      _checkPointState(i);
      _send(_digitsToShow[i]);
    }
    _stop();
  }
  
  void _convert(uint16_t val)
  {
    uint16_t k = 1000;
    for (uint8_t i = 0, j = 0; i < 4; i++)
    {
      j = val / k;
      _digitsToShow[i] = _codesOfDigits[j];
      val -= j * k;
      k /= 10;
    }
  }

  void _init()
  {
    _sendByte(CMD_MODE);
    _updateDisplay();
    setBrightness(_brightness);
  }


  uint8_t _dio, _clk;

  bool _pointState = false;
  uint8_t _brightness = 1;
  uint8_t _digitsToShow[4] = { 0b01000000, 0b01000000, 0b01000000, 0b01000000 };
  const uint8_t _codesOfDigits[12] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111100, 0b00000111, 0b01111111, 0b01100111 },
                         _chars[2] = { 0b00000000, 0b01000000 };   // ' ', '-'


  enum
  {
    CMD_MODE   = 0b01000000,  // 0x40,  авто инкремент разряда
    CMD_FIXED  = 0b01000100,  // 0x44,  фиксированный режим (к CMD_DATA прибавть номер разряда 0-3)
    CMD_DATA   = 0b11000000,  // 0xC0,  адрес первого разряда
    CMD_BRIGHT = 0b10001000,  // 0x88,  сюда прибавить уровень яркости
    CMD_OFF    = 0b10000000   // 0x80,  офнуть показ цифр (или полностью???)
  };

  
};

