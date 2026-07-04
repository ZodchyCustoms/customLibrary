#include <avr/io.h>
//#include <customs/pins.h>

class Encoder {
public:
    Encoder(uint8_t _s1, uint8_t _s2, uint8_t _key) {
        s1 = _s1;
        s2 = _s2;
        key = _key;
        setPinInput(s1);
        setPinInput(s2);
        setPinInput(key);
        
    }



    uint8_t s1, s2, key, mask;

private:

    uint8_t _code = 0;

};