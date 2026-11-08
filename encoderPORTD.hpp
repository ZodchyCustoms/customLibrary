#pragma once
#include <avr/io.h>

class EncoderPORTD
{
public:
  // Прописать PCINT2_vect вручную!
  EncoderPORTD(uint8_t s1, uint8_t s2, uint8_t key)
  {
    this->_s1 = s1;
    this->_s2 = s2;
    this->_key = key;
    DDRD &= ~((1 << _s1) | (1 << _s2) | (1 << _key));
    PORTD |= (1 << _s1) | (1 << _s2) | (1 << _key);
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << _s1) | (1 << _s2) | (1 << _key);
  }

  ~EncoderPORTD()
  {
    DDRD &= ~( (1 << _s1) | (1 << _s2) | (1 << _key) );
    PCICR &= ~(1 << PCIE2);
    PCMSK2 &= ~( (1 << _s1) | (1 << _s2) | (1 << _key) );
  }

  enum class State : uint8_t
  {
    wait,
    rotateLeft,
    rotateRight,
    press
  };

  State readState()
  {
    State state = _state;
    _state = State::wait;
    return state;
  }

  uint8_t readCode()
  {
    return _code;
  }

  void interruptRoutine()
  {
    _code <<= 2;
    _code |= ( (PIND >> _s1) & 1 | ((PIND >> _s2) & 1) << 1 );
    _isPress = (PIND >> _key) & 1;  // Значение инвертировано!
    switch (_code)
    {
      case 11:
      _state = State::rotateRight;
      break;
      case 75:
      _state = State::rotateRight;
      break;
      case 135:
      _state = State::rotateLeft;
      break;
      default:
      break;
    }
    switch (_isPress)
    {
      case false:   // Кнопка нажата
      switch (_wasPressed)
      {
        case false:
        _wasPressed = true;
        _state = State::press;
        case true:
        break;
      }
      break;
      case true:
      switch (_wasPressed)
      {
        case true:
        _wasPressed = false;
        break;
        case false:
        break;
      }
      break;
    }
  }

private:
  volatile bool _isPress, _wasPressed;
  volatile uint8_t _code;
  volatile State _state = State::wait;
  uint8_t _s1, _s2, _key;

};

