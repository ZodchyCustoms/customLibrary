#pragma once
#include <stdint.h>
#include <avr/interrupt.h>

#define TX_BUFFER_SIZE 200
#define RX_BUFFER_SIZE 50
#define NUMBERS_BUFFER_SIZE 13


class Uart
{
public:
  static Uart& getInstance()
  {
    static Uart instance;
    return instance;
  }

  void init(uint32_t baudRate)
  {
    uint8_t p = 16;
    if (baudRate >= 115200)
    {
      p = 8;
      UCSR0A |= (1 << U2X0);
    }
    _baudRateValue = F_CPU / p / baudRate - 1;
    UBRR0H = _baudRateValue >> 8;
    UBRR0L = _baudRateValue;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    sei();
    
  }

  void print(const char *message)
  {
    _putInTxBuffer(message);
    _startTransmit();
  }
  
  void print(const char ch)
  {
    _txBufferWrite(ch);
    _startTransmit();
  }
  
  void print(int32_t number)
  {
    _putInTxBuffer(_convertNumbers(number));
    _startTransmit();
  }

  bool available()
  {
    return _bytesRecieved;
  }
  
  // Переделать на return const char*
  void read()
  {
    print(_readFromRxBuffer());
  }
  
  void rxEnable()
  {
    UCSR0B |= (1 << RXCIE0);
  }
  

  // need to make a complite interrupt mode
  void uartWriteInUDR0();
  void uartReadFromUDR0();

private:
  Uart() = default;

  void _startTransmit()
  {
    if (!_busy && _bytesToRead > 0)
    {
      _busy = true;
      UCSR0B |= (1 << UDRIE0);
    }
  }
  
  char* _convertNumbers(int32_t number)
  {
    bool _minus = false;
    uint8_t i = NUMBERS_BUFFER_SIZE - 1;
    if (number < 0)
    {
      number *= -1;
      _minus = true;
    }
    if (number == 0)
    {
      _numbers[i--] = '0';
    }
    else
    {
      while (number)
      {
        _numbers[i--] = number % 10 + '0';
        number /= 10;
      }
    }
    if (_minus)
      _numbers[i--] = '-';
    return &_numbers[i + 1];
  }
  
  void _txBufferWrite(char byte)
  {
    _txBuffer[_tx_w++] = byte;
    _bytesToRead++;
    if (_tx_w >= TX_BUFFER_SIZE)
    {
      _tx_w = 0;
      // TODO: add actions if OVF
    }
  }
  
  void _putInTxBuffer(const char *byte)
  {
    while (*byte)
    {
      _txBufferWrite(*byte++);
    }
  }
  
  char* _readFromRxBuffer()
  {
    uint8_t i = 0;
    while (_bytesRecieved > 0 && i < RX_BUFFER_SIZE)
    {
      _message[i++] = _rxBuffer[_rx_r++];
      if (_rx_r == RX_BUFFER_SIZE)
        _rx_r = 0;
      _bytesRecieved--;
    }
    _message[i] = '\0';
    return _message;
  }

  // #################### INTERRUPT ROUTINES #########################

  void _writeInUDR0()
  {
    if (_bytesToRead > 0)
    {
      UDR0 = _txBuffer[_tx_r++];
      if (_tx_r == TX_BUFFER_SIZE)
        _tx_r = 0;
      _bytesToRead--;
    } else
    {
      UCSR0B &= ~(1 << UDRIE0);
      _busy = false;
    }
  }
  
  void _readFromUDR0()
  {
    _rxBuffer[_rx_w++] = UDR0;
    _bytesRecieved++;
    if (_rx_w == RX_BUFFER_SIZE)
    {
      _rx_w = 0;
      // TO DO: actions if got OVF
    }
  }

  // ########################## VARIABLES #############################

  uint16_t _baudRateValue;
// for transmit:
  char _txBuffer[TX_BUFFER_SIZE] = {'\0'};
  char _numbers[NUMBERS_BUFFER_SIZE + 1] = {'\0'}; // size + 1 for '\0' - last symbol
  uint8_t _tx_r = 0;
  uint8_t _tx_w = 0;
  volatile uint16_t _bytesToRead = 0;
  bool _busy = false;
  // for recieve:
  char _rxBuffer[RX_BUFFER_SIZE] = {'\0'};
  char _message[RX_BUFFER_SIZE + 1] = {'\0'};
  uint8_t _rx_r = 0;
  uint8_t _rx_w = 0;
  uint8_t _bytesRecieved = 0;

  friend void udreInterruptRoutine();
  friend void rxInterruptRoutine();

};

Uart uart = Uart::getInstance();

void udreInterruptRoutine()
{
  uart._writeInUDR0();
}

void rxInterruptRoutine()
{
  uart._readFromUDR0();
}

ISR(USART_UDRE_vect)
{
  udreInterruptRoutine();
}

ISR(USART_RX_vect)
{
  rxInterruptRoutine();
}

