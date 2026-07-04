#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <customize/pins.h>

/*
Число для отображения хранится в массиве _digitsToShow[4] в виде кодов чисел.
Функция _showDigitals() отправляет их на экран - каждый байт это цифра (код).
Для включения двоеточия необходимо установить старший бит во всех и отправить байты.
Для выбора разряда необходимо добавить его номер к CMD_DATA.
*/


class TM1637 {
public:
  TM1637(uint8_t dio, uint8_t clk)
  {
    this->_dio = dio;
    this->_clk = clk;
    setPin(_dio);
    setPin(_clk);
    _hello();
  }

  // convert value and show on the display
  void show(uint16_t value)
  {
    // convert:
    _convert(value);
    // and show
    _showDigitals();
  }

  void showChar(uint8_t numberOfChar)
  {
    for (uint8_t i = 0; i < 4; i++)
    {
      _digitsToShow[i] = _chars[numberOfChar];
    }
    _showDigitals();
  }

  void setBrightness(uint8_t level)
  {
    if (level > 7)
      level = 7;
    _start();
    _send(CMD_BRIGHT + level);
    _stop();
  }

  void point(bool state)
  {
    _pointState = state;
    _showDigitals();
  }

  /*
  void point(bool state)
  {
    switch (state)
    {
      case true:
      for (uint8_t i = 0; i < 4; i++)
      _digitsToShow[i] |= (1 << 7);
      break;
      case false:
      for (uint8_t i = 0; i < 4; i++)
      _digitsToShow[i] &= ~(1 << 7);
    }
    _showDigitals();
  }
  */

  void setTime(uint8_t value, uint8_t razr)
  {
    uint8_t i, j;
    switch (razr)
    {
      case RAZR_MIN:
      i = 0;
      j = 1;
      break;
      case RAZR_SEC:
      i = 2;
      j = 3;
      break;
    }
    _digitsToShow[i] = _digits[value / 10];
    _digitsToShow[j] = _digits[value % 10];
    _showDigitals();
  }

/*
  void razrState(uint8_t razr, bool state)
  {
    switch (state)
    {
      case true:
      _digitsToShow[razr] = _digitsToShowOld[razr];
      break;
      case false:
      _digitsToShow[razr] = _digits[CHAR_OFF];
      break;
    }
    _showDigitals();
  }
*/
  
  enum  // Выбранный разряд (при установке времени)
  {
    RAZR_SEC = 0,
    RAZR_MIN = 1
  };

private:

  void _start()
  {
    pinWrite(_dio, true);
    pinWrite(_clk, true);
    _delay_us(2);
    pinWrite(_dio, false);
    _delay_us(2);
    pinWrite(_clk, false);
  }

  void _stop()
  {
    pinWrite(_dio, false);
    pinWrite(_clk, false);
    _delay_us(2);
    pinWrite(_clk, true);
    _delay_us(2);
    pinWrite(_dio, true);
  }

  void _strobe()
  {
    _delay_us(2);
    pinWrite(_clk, true);
    _delay_us(2);
    pinWrite(_clk, false);
    _delay_us(2);
  }

  // just send a byte of data and waiting for ACK
  void _send(uint8_t value)
  {
    for (uint8_t i = 0; i < 8; i++)
    {
      pinWrite(_dio, (value & 1));
      _strobe();
      value >>= 1;
    }
    setPinInput(_dio);
    _delay_us(2);
    pinWrite(_clk, true);
    while(!pinRead(_dio)) {}
    pinWrite(_clk, false);
    setPin(_dio);
  }

  // start -> send -> stop
  void _sendByte(uint8_t data)
  {
    _start();
    _send(data);
    _stop();
  }

  // update display
  void _showDigitals()
  {
    _start();
    _send(CMD_DATA);
    for (uint8_t i = 0; i < 4; i++)
    {
      switch (_pointState)
      {
        case true:
        _digitsToShow[i] |= (1 << 7);
        break;
        case false:
        _digitsToShow[i] &= ~(1 << 7);
        break;
      }
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
      _digitsToShow[i] = _digits[j];
      //_digitsToShow[i] |= (_pointState << 7);      // for save points state
      // _digitsToShowOld[i] = _digitsToShow[i];   // for use with razrState()
      val -= j * k;
      k /= 10;
    }
  }

  // Start animation
  void _hello()
  {
    _sendByte(CMD_MODE);
    _showDigitals();
    setBrightness(1);

    // Anim
    cli();
    uint8_t i = 0, j = 0;
    uint16_t _pause = 150;
    for (; i < 4; i++)
    {
      _digitsToShow[i] |= (1 << j);
      _showDigitals();
      _delay_ms(_pause);
    }
    j++;
    _digitsToShow[3] |= (1 << j++);
    _showDigitals();
    _delay_ms(_pause);
    _digitsToShow[3] |= (1 << j++);
    _showDigitals();
    _delay_ms(_pause);
    for (; i != 255; i--)
    {
      _digitsToShow[i] |= (1 << j);
      _showDigitals();
      _delay_ms(_pause);
    }
    j++;
    _digitsToShow[0] |= (1 << j++);
    _showDigitals();
    _delay_ms(_pause);
    _digitsToShow[0] |= (1 << j);
    _showDigitals();
    _delay_ms(_pause);
    i = 0;
    for (; i < 4; i++)
    {
      _digitsToShow[i] = CHAR_MINUS;
      _digitsToShowOld[i] = _digitsToShow[i];
    }
    _showDigitals();
    sei();
  }


  uint8_t _dio, _clk;

  volatile bool _pointState = true;
  volatile uint8_t _digitsToShow[4] = { 0, 0, 0, 0 },
                _digitsToShowOld[4] = { 0, 0, 0, 0 };
  const uint8_t _digits[12] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111100, 0b00000111, 0b01111111, 0b01100111 },
                  _chars[3] = { 0b00000000, 0b01000000, 0b00000000 };   // ' ', '-'


  enum
  {
    CMD_MODE = 0b01000000,    // 0x40,
    CMD_DATA = 0b11000000,    // 0xC0,  адрес первого разряда
    CMD_BRIGHT = 0b10001000,  // 0x88,
    CMD_FIXED = 0b01000100,   // 0x44,
    CMD_OFF = 0b10000000      // 0x80
  };

  enum
  {
    CHAR_OFF = 10,
    CHAR_MINUS = 11
  };

  
};

