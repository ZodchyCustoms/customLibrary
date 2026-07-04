#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <customs/pins.h>

#define SCL_CLOCK 100000L

void twiInit()
{
    TWBR = ( (F_CPU / SCL_CLOCK) - 16) / 2;
    TWSR = 0;
}

void twiInitSlave(uint8_t slaveAddr)
{
  TWBR = ( (F_CPU / SCL_CLOCK) - 16) / 2;
  TWSR = 0;
  TWAR = slaveAddr << 1;
  TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);
  sei();
}

void twiStart()
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while ( !(TWCR & (1 << TWINT)) ) { ; }
}

void twiStop()
{
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void twiWrite(uint8_t data)
{
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ( !(TWCR & (1 << TWINT)) ) { ; }
}

uint8_t twiReadAck()
{
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while ( !(TWCR & (1 << TWINT))) { ; }
    return TWCR;
}

uint8_t twiReadNack()
{
    TWCR = (1 << TWEN) | (1 << TWINT);
    while ( !(TWCR & (1 << TWINT))) { ; }
    return TWCR;
}



/*

#define SCL_CLOCK 100000L

void twiInit()
{
    TWBR = ( (F_CPU / SCL_CLOCK) - 16) / 2;
    TWSR = 0;
}

void twiInitSlave(uint8_t slaveAddr)
{
  TWBR = ( (F_CPU / SCL_CLOCK) - 16) / 2;
  TWSR = 0;

  TWAR = slaveAddr << 1;
  TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);
  sei();
}

void twiStart()
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while ( !(TWCR & (1 << TWINT)) ) { ; }
}

void twiStop()
{
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void twiWrite(uint8_t data)
{
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while ( !(TWCR & (1 << TWINT)) ) { ; }
}

uint8_t twiReadAck()
{
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while ( !(TWCR & (1 << TWINT))) { ; }
    return TWCR;
}

uint8_t twiReadNack()
{
    TWCR = (1 << TWEN) | (1 << TWINT);
    while ( !(TWCR & (1 << TWINT))) { ; }
    return TWCR;
}

*/