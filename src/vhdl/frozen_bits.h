#ifndef frozen_bits_header
#define frozen_bits_header


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <iostream>
#include <map>
#include <math.h>
#include <vector>

#include "../RawSound.h"

using namespace std;

class frozen_bits {
private:
    int eFreq;
    vector<int> liste;

public:
    frozen_bits(RawSound* data, int rLength, int rFreq, int channels, int rBits);
    int size();
    int at(int pos);
};

#endif
