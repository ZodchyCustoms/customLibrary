//#include <customs/pins.h>

// добавить режим отображения только значимых цифр

class Indicator {
public:
    Indicator(uint8_t _dataPin, uint8_t _latchPin, uint8_t _clockPin) {
        dataPin = _dataPin;
        latchPin = _latchPin;
        clockPin = _clockPin;
        setPin(dataPin);
        setPin(latchPin);
        setPin(clockPin);
        pinWrite(dataPin, 0);
        pinWrite(latchPin, 0);
        pinWrite(clockPin, 0);
    }

    volatile uint8_t dataPin, latchPin, clockPin;
    volatile uint16_t indicateValue;

    void indicate()
    {
        static uint8_t iteration = 0;
        bitOutDouble(iteration++);
        //iteration++;
        if (iteration == 4)
        {
            iteration = 0;
            valueToNums();
        }
    }


private:

    const uint8_t digit[4] = { 0b11111110, 0b11111101, 0b11111011, 0b11110111 },
               symbols[10] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
                               0b01101101, 0b01111100, 0b00000111, 0b01111111, 0b01100111 };
    volatile uint8_t symbolIndex[4] = { 1, 2, 3, 4 };

    void valueToNums()
    {
        static uint16_t k, value;
        value = indicateValue;
        k = 1000;
        for (uint8_t i = 0; i < 4; i++)
        {
            symbolIndex[i] = value / k;
            value -= symbolIndex[i] * k;
            k /= 10;
        }
    }

    void bitOutDouble(uint8_t ind)
    {
        static uint16_t bits;
        bits = ( symbols[symbolIndex[ind]] << 8) + digit[ind];

        pinWrite(latchPin, 0);
        for (uint8_t i = 0; i < 16; i++)
        {
            pinWrite(dataPin, bits & 1);
            pinWrite(clockPin, 1);
            pinWrite(clockPin, 0);
            bits >>= 1;
        }
        pinWrite(latchPin, 1);
    }

};

