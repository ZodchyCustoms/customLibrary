#include <avr/interrupt.h>



/*  
*   stock = 62500 ovfs/sec
*   
*   
*   
*/  

uint16_t toneValue, OVFcounter = 0;

void stock_1ms()
{
    TCCR2A = 0;         //  normal mode
    TCCR2B = 1;         //  prescaller = 0

}

// need to call TIMER2_COMPA_vect
void timing_1ms()
{
    TCCR2A = 2;
    TCCR2B = 4;
    TIMSK2 = 2;
    OCR2A = 249;
}

void timing_tone()
{
    TCCR2A = 2;
    TCCR2B = 1;
    //TIMSK2 |= (1 << TOIE2);// | (1 << OCIE2A);
    TIMSK2 |= (1 << OCIE2A);
    OCR2A = 128;
}

// timer0 SETUP
void setTimer0()
{
    TCCR0A = 0;
    TCCR0B = 1;
    TIMSK0 |= (1 << OCIE0A);
    OCR0A = 127;
}

// timer0 going tone()
void tone(uint8_t value)
{
    
}



