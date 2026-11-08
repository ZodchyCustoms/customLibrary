#pragma once


void setPinOutput(uint8_t pin)
{
  switch (pin)
  {
    default:
    break;
    case 0 ... 7:
    pin -= 0;
    DDRD |= (1 << pin);
    break;
    case 8 ... 13:
    pin -= 8;
    DDRB |= (1 << pin);
    break;
    case 14 ... 19:
    pin -= 14;
    DDRC |= (1 << pin);
    break;
  };
}

// pull-up
void setPinInput(uint8_t pin)
{
  switch (pin)
  {
    default:
    break;
    case 0 ... 7:
    pin -= 0;
    DDRD &= ~(1 << pin);
    PORTD |= (1 << pin);
    break;
    case 8 ... 13:
    pin -= 8;
    DDRB &= ~(1 << pin);
    PORTB |= (1 << pin);
    break;
    case 14 ... 19:
    pin -= 14;
    DDRC &= ~(1 << pin);
    PORTC |= (1 << pin);
    break;
  };
}

void setPin(uint8_t pin, bool state)
{
  switch (pin)
  {
    default:
    break;
    case 0 ... 7:
    pin -= 0;
    switch (state)
    {
      case true:
      PORTD |= (1 << pin);
      break;
      case false:
      PORTD &= ~(1 << pin);
      break;
    };
    break;
    case 8 ... 13:
    pin -= 8;
    switch (state)
    {
      case true:
      PORTB |= (1 << pin);
      break;
      case false:
      PORTB &= ~(1 << pin);
      break;
    };
    break;
    case 14 ... 19:
    pin -= 14;
    switch (state)
    {
      case true:
      PORTC |= (1 << pin);
      break;
      case false:
      PORTC &= ~(1 << pin);
      break;
    };
    break;
  };
}

void togglePin(uint8_t pin)
{
  switch (pin)
  {
    default:
    break;
    case 0 ... 7:
    pin -= 0;
    PORTD ^= (1 << pin);
    break;
    case 8 ... 13:
    pin -= 8;
    PORTB ^= (1 << pin);
    break;
    case 14 ... 19:
    pin -= 14;
    PORTC ^= (1 << pin);
    break;
  };
}

// return inverted value of PINx
bool readPin(uint8_t pin)
{
  switch (pin)
  {
    default:
    return false;
    break;
    case 0 ... 7:
    pin -= 0;
    return !( PIND & (1 << pin) );
    break;
    case 8 ... 13:
    pin -= 8;
    return !( PINB & (1 << pin) );
    break;
    case 14 ... 19:
    pin -= 14;
    return !( PINC & (1 << pin) );
    break;
  };
}