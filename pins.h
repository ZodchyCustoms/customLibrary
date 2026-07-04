#pragma once
#include <avr/io.h>

// пин на выход
void setPin(uint8_t pin) {
  switch (pin) {
    case 0 ... 7:
      DDRD |= (1 << pin);
      break;
    case 8 ... 13:
      pin -= 8;
      DDRB |= (1 << pin);
      break;
  }
}

//пин на вход PULL_UP
void setPinInput(uint8_t pin) {
  switch (pin) {
    case 0 ... 7:
      DDRD &= ~(1 << pin);
      PORTD |= (1 << pin);
      break;
    case 8 ... 13:
      pin -= 8;
      DDRB &= ~(1 << pin);
      PORTB |= (1 << pin);
      break;
  }
}

void portD(uint8_t pinD, bool pinState)
{
  switch (pinState) {
    case 0:
      PORTD &= ~(1 << pinD);
      break;
    case 1:
      PORTD |= (1 << pinD);
      break;
  }
}

void portB(uint8_t pinB, bool pinState)
{
  switch (pinState) {
    case 0:
      PORTB &= ~(1 << pinB);
      break;
    case 1:
      PORTB |= (1 << pinB);
      break;
  }
}

void pinWrite(uint8_t number, bool state)
{
  static uint8_t _pin;
  number >= 8 ? _pin = number - 8 : _pin = number + 0;
  switch (number) {
    case 0 ... 7:
      portD(_pin, state);
      break;
    case 8 ... 13:
      portB(_pin, state);
      break;
  }
}

// проверить везде!
bool pinRead(uint8_t number)
{
  static uint8_t _pinToRead;
  bool pinState = 0;
  number >= 8 ? _pinToRead = number - 8 : _pinToRead = number;
  switch (number) {
    case 0 ... 7:
    pinState = ( PIND & (1 << _pinToRead) );
    break;
    case 8 ... 13:
    pinState = ( PINB & (1 << _pinToRead) );
    break;
  }
  return !pinState;
}





// __________________________________not use___
/*

void bitOut(uint8_t bits, uint8_t pinD, uint8_t pinC)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        pinWrite(pinD, bits & 1);
        pinWrite(pinC, 1);
        pinWrite(pinC, 0);
        bits >>= 1;
    }
}

void pinOn(uint8_t pin) {
  switch (pin) {
    case 0 ... 7:
    PORTD |= (1 << pin);
    break;
    case 8 ... 13:
    pin -= 8;
    PORTB |= (1 << pin);
    break;
  }
}

void pinOff(uint8_t pin) {
  switch (pin) {
    case 0 ... 7:
      PORTD &= ~(1 << pin);
      break;
      case 8 ... 13:
      pin -= 8;
      PORTB &= ~(1 << pin);
      break;
    }
  }


*/

