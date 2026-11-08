#include <avr/io.h>
#include <util/delay.h>
#include <customs/pins.h>

enum cmds {
    writeData = 0x40,
    addrAuto = 0x40,    // 64, DEC
    addrFirst = 0xc0,
    dispON = 0x80,
    brightnessStock = 0x88,
    point = 0 | (1 << 7)
};

class TM1637 {
public:
    TM1637(uint8_t _clkPin, uint8_t _dioPin) {
        clkPin = _clkPin;
        dioPin = _dioPin;
        setPin(clkPin);
        setPin(dioPin);
        pinWrite(clkPin, 1);
        pinWrite(dioPin, 1);
        //indicate();
    }

    bool pointFlag = false;
    uint8_t nums[12] = { 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110,
                         0b01101101, 0b01111100, 0b00000111, 0b01111111, 0b01100111, 0b00000000, 0b01000000 };   // 0 - 9, , -, 
    uint8_t numIndex[4] = { 0b01111100, 0b00000111, 0b01111111, 0b01100111 };
    
    uint8_t digsToShow[4] = { 0, 0, 0, 0 },
         oldDigsToShow[4] = { 0, 0, 0, 0 };
    
    uint8_t seconds, minutes, brigh = 1;


    void show(uint16_t value)
    {
        //converting:
        uint16_t k = 1000;
        uint8_t i = 0, ind = 0;
        for (i = 0; i < 4; i++)
        {
            ind = value / k;
            digsToShow[i] = nums[ind];
            value -= ind * k;
            k /= 10;
        }
        //show:
        start();
        sendByte(writeData);
        stop();

        start();
        sendByte(addrFirst);
        
        for (i = 0; i < 4; i++)
        {
            if (pointFlag)
                sendByte(digsToShow[i] | point);
            else
                sendByte(digsToShow[i]);
        }
        
        stop();

        start();
        sendByte(brighness + brigh);
        stop();
    }

    void upd(bool _point)
    {
        if (_point)
        {
            for (uint8_t i = 0; i < 4; i++)
                digsToShow[i] |= point;
        }
        indicate();
    }

    void minus()
    {
        for (uint8_t i = 0; i < 4; i++)
            digsToShow[i] = nums[11];
        indicate();
    }



private:

    
    uint8_t brighness = brightnessStock;
    uint8_t clkPin, dioPin;

    //for use with convert()
    void indicate()
    {
        start();
        sendByte(writeData);
        stop();

        start();
        sendByte(addrFirst);
        
        for (uint8_t i = 0; i < 4; i++)
        {
            sendByte(digsToShow[i]);
        }
        
        stop();

        start();
        sendByte(brighness + 1);
        stop();
    }

    //SRAM data
    void sramData()
    {
        // set data             1
        start();
        sendByte(64);
        stop();

        // set address          2
        start();
        sendByte(192);                              //...192 - left dig...
        // data 1 - N           3
        for (uint8_t i = 0; i < 4; i++)
        {
            //sendByte(digits[i + 5]);
            sendByte(nums[i]);
        }
        
        stop();
        // control display      4
        start();
        sendByte(brighness + 2);  // brighness        //...143 - full...
        stop();
    }


    void start()    //
    {
        pinWrite(clkPin, 1);
        pinWrite(dioPin, 1);
        pinWrite(dioPin, 0);
        pinWrite(clkPin, 0);
    }
    
    void stop()     //
    {
        pinWrite(clkPin, 0);
        pinWrite(dioPin, 0);
        pinWrite(clkPin, 1);
        pinWrite(dioPin, 1);
    }

    void sendByte(uint8_t data)     //
    {
        // LSB first
        uint8_t i = 0;
        uint8_t ack = 0;

        for (i = 0; i < 8; i++)
        {
            pinWrite(clkPin, 0);
            if (data & 1)
                pinWrite(dioPin, 1);
             else 
                pinWrite(dioPin, 0);
            data >>= 1;
            pinWrite(clkPin, 1);
        }

        pinWrite(clkPin, 0);            // waiting ACK
        pinWrite(dioPin, 0);
        setPinInput(dioPin);
        pinWrite(clkPin, 1);
        while (pinRead(dioPin)) { ; }
        pinWrite(clkPin, 0);
        setPin(dioPin);
    }

};


    // BUG there:
    /*
    void converter1(uint16_t value)
    {
        for (uint8_t i = 3; i >= 0; i--)
        {
            numIndex[i] = value % 10;
            value /= 10;
        }
        indicate();
    }
    */


    // reserved:
    /*
    void converter(uint16_t value)
    {
        uint16_t k = 1000;
        for (uint8_t i = 0; i < 4; i++)
        {
            numIndex[i] = value / k;
            value -= numIndex[i] * k;
            k /= 10;
        }
        indicate();
    }
*/



