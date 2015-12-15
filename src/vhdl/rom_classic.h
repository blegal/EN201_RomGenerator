#ifndef rom_classic_header
#define rom_classic_header

#include "frozen_bits.h"

class rom_classic {
private:
    frozen_bits* fbits;
    int yPos;
    int nBits;
    int frequency;

public:
    rom_classic(frozen_bits* _fbits, int _nBits, int _frequency);
    void run();
};

#endif
