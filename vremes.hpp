#include <avr/interrupt.h>

namespace Vremes
{
  bool activate()
  {
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    TCCR1B |= (1 << CS10);
    TIMSK1 |= (1 << TOIE1);
    return true;
  }

  volatile uint32_t overflows = 0;

  bool vremes = activate();
};


uint32_t micros()
{
  cli();
  uint32_t us = ((Vremes::overflows * 65535) + TCNT1) / 16;
  sei();
  return us;
}

uint32_t millis()
{
  cli();
  uint32_t ms = ((Vremes::overflows * 65535) + TCNT1) / 16000;
  sei();
  return ms;
}

ISR(TIMER1_OVF_vect)
{
  Vremes::overflows++;
}
