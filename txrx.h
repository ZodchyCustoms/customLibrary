#include <avr/io.h>
#include <avr/interrupt.h>

class Txrx {
public:
    Txrx(uint16_t _baudRate) {
        baudRate = _baudRate;
        cli();
        init();
        sei();
    }


    void send(uint8_t ch)
    {
        while ( !(UCSR0A & (1 << UDRE0)) ); { ; }
        UDR0 = ch;
    }

    void print(char* arr)
    {
        data = *arr++;
        while (data)
        {
            send(data);
            data = *arr++;
        }
    }

    void print(uint8_t* numArr, uint8_t len)
    {
      for (i = 0; i < len; i++)
      {
        send(numArr[i]); 
      }
    }

private:

    uint8_t data, i = 0;
    uint16_t bauds, baudRate;

    void init()
    {
        //bauds = (F_CPU / 8 / baudRate - 1) / 2;
        bauds = (F_CPU / 16 / baudRate) - 1;
        UBRR0H = (bauds >> 8);
        UBRR0L = bauds;
        UCSR0B = (1 << RXEN0) | (1 << TXEN0);
        UCSR0C = (1 << USBS0) | (3 << UCSZ00);
    }

    void off()
    {
        UCSR0B &= ~(1 << TXEN0) | ~(1 << RXEN0);  //  ?
    }

};




/*
_______________________________RESERV:________________________________________


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/delay.h>

void usartInit(uint16_t baud);
void usartTx(uint8_t data);
void transmitting();

int main()
{
  DDRB = 255;
  uint16_t b = 9600;
  usartInit(b);
  sei();
  
  
  while (true) {
    _delay_ms(1000);
    transmitting();
  };
  return 0;
}

void usartInit(uint16_t baud)
{
  uint16_t baudSet = (F_CPU / 8 / baud - 1) / 2;  //  ?
  UBRR0H = (baudSet >> 8);
  UBRR0L = baudSet;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void usartTx(uint8_t data)
{
  while ( ! (UCSR0A & (1 << UDRE0)) ) { ; }
  UDR0 = data;
}

void transmitting()
{
  static uint8_t val = 'a';
  usartTx(val);
  val++;
  PORTB ^= (1 << 5);
}

____________________________________________________________________________

*/


/*

___TRASH_BOX___

#include <Arduino.h>

uint8_t ubrr0, ubrr0h, ubrr0l, ucsr0a, ucsr0b, ucsr0c, udr0;



void setup() {
  ubrr0 = UBRR0;
  ubrr0h = UBRR0H;
  ubrr0l = UBRR0L;
  ucsr0a = UCSR0A;
  ucsr0b = UCSR0B;
  ucsr0c = UCSR0C;
  udr0 = UDR0;

  Serial.begin(115200);

  Serial.println("__before start: ");
  Serial.print("UDR0 = ");
  Serial.println(udr0, BIN);
  Serial.print("UCSR0A = ");
  Serial.println(ucsr0a, BIN);
  Serial.print("UCSR0B = ");
  Serial.println(ucsr0b, BIN);
  Serial.print("UCSR0C = ");
  Serial.println(ucsr0c, BIN);
  Serial.print("UBRR0H = ");
  Serial.println(ubrr0h, BIN);
  Serial.print("UBRR0L = ");
  Serial.println(ubrr0l, BIN);
  Serial.print("UBRR0 = ");
  Serial.println(ubrr0, BIN);

  ubrr0 = UBRR0;
  ubrr0h = UBRR0H;
  ubrr0l = UBRR0L;
  ucsr0a = UCSR0A;
  ucsr0b = UCSR0B;
  ucsr0c = UCSR0C;
  udr0 = UDR0;

  Serial.println("__after start:  ");

  Serial.print("UDR0 = ");
  Serial.println(udr0, BIN);
  Serial.print("UCSR0A = ");
  Serial.println(ucsr0a, BIN);
  Serial.print("UCSR0B = ");
  Serial.println(ucsr0b, BIN);
  Serial.print("UCSR0C = ");
  Serial.println(ucsr0c, BIN);
  Serial.print("UBRR0H = ");
  Serial.println(ubrr0h, BIN);
  Serial.print("UBRR0L = ");
  Serial.println(ubrr0l, BIN);
  Serial.print("UBRR0 = ");
  Serial.println(ubrr0, BIN);

  Serial.end();

  ubrr0 = UBRR0;
  ubrr0h = UBRR0H;
  ubrr0l = UBRR0L;
  ucsr0a = UCSR0A;
  ucsr0b = UCSR0B;
  ucsr0c = UCSR0C;
  udr0 = UDR0;

  Serial.begin(115200);

  Serial.print("__before end: ");
  Serial.print("UDR0 = ");
  Serial.println(udr0, BIN);
  Serial.print("UCSR0A = ");
  Serial.println(ucsr0a, BIN);
  Serial.print("UCSR0B = ");
  Serial.println(ucsr0b, BIN);
  Serial.print("UCSR0C = ");
  Serial.println(ucsr0c, BIN);
  Serial.print("UBRR0H = ");
  Serial.println(ubrr0h, BIN);
  Serial.print("UBRR0L = ");
  Serial.println(ubrr0l, BIN);
  Serial.print("UBRR0 = ");
  Serial.println(ubrr0, BIN);

  Serial.end();
  Serial.begin(9600);

  ubrr0 = UBRR0;
  ubrr0h = UBRR0H;
  ubrr0l = UBRR0L;
  ucsr0a = UCSR0A;
  ucsr0b = UCSR0B;
  ucsr0c = UCSR0C;
  udr0 = UDR0;

  Serial.end();

  Serial.begin(115200);

  Serial.print("__before end 9600: ");
  Serial.print("UDR0 = ");
  Serial.println(udr0, BIN);
  Serial.print("UCSR0A = ");
  Serial.println(ucsr0a, BIN);
  Serial.print("UCSR0B = ");
  Serial.println(ucsr0b, BIN);
  Serial.print("UCSR0C = ");
  Serial.println(ucsr0c, BIN);
  Serial.print("UBRR0H = ");
  Serial.println(ubrr0h, BIN);
  Serial.print("UBRR0L = ");
  Serial.println(ubrr0l, BIN);
  Serial.print("UBRR0 = ");
  Serial.println(ubrr0, BIN);

  Serial.end();

  ubrr0 = UBRR0;
  ubrr0h = UBRR0H;
  ubrr0l = UBRR0L;
  ucsr0a = UCSR0A;
  ucsr0b = UCSR0B;
  ucsr0c = UCSR0C;
  udr0 = UDR0;

  Serial.begin(115200);

  Serial.print("__before end: ");
  Serial.print("UDR0 = ");
  Serial.println(udr0, BIN);
  Serial.print("UCSR0A = ");
  Serial.println(ucsr0a, BIN);
  Serial.print("UCSR0B = ");
  Serial.println(ucsr0b, BIN);
  Serial.print("UCSR0C = ");
  Serial.println(ucsr0c, BIN);
  Serial.print("UBRR0H = ");
  Serial.println(ubrr0h, BIN);
  Serial.print("UBRR0L = ");
  Serial.println(ubrr0l, BIN);
  Serial.print("UBRR0 = ");
  Serial.println(ubrr0, BIN);

  Serial.end();


}

void loop() {
  
}

*/