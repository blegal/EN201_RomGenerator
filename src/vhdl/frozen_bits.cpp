#include "frozen_bits.h"
#include "rom_classic.h"

/**
 *
 *
 *
 */
frozen_bits::frozen_bits(RawSound* data, int rLength, int rFreq, int channels, int rBits)
{
    unsigned int frequence = data->sample_rate();
    unsigned int length    = data->length();
    unsigned int step      = (frequence/rFreq) * channels;

    std::cout << "################ frozen_bits ####################" << endl;
    std::cout << "(II) Audio file real frequency : " << frequence    << std::endl;
    std::cout << "(II) Audio file req. frequency : " << rFreq        << std::endl;
    std::cout << "(II) Comp. subsampling factor  : " << step         << std::endl;
    std::cout << "(II) Audio file real length    : " << length       << std::endl;
    std::cout << "(II) Audio file sub. length    : " << rLength/step << std::endl;

    rLength = (rLength >= (length/step)) ? (length/step) : rLength;

    //
    // ON CALCULE L'AMPLITUDE MINIMUM ET MAXIMUM DES VALEURS
    // CONTENUES DANS LES DONNEES AUDIO
    //
    short* values = data->data();
    for(unsigned int q=0; q<rLength; q+=1){
        int value = (values[step * q]);
        liste.push_back( value );
    }

    int min = liste[0];
    int max = liste[0];
    for(int q = 1; q < rLength; q+=1) // On ne recupere que les échantillons qui nous interessent
    {
        min = (min < liste[q]) ? min : liste[q];
        max = (max > liste[q]) ? max : liste[q];
    }

    std::cout << "################ frozen_bits ####################" << endl;
    std::cout << "(II) min signal value detected : " << min << std::endl;
    std::cout << "(II) max signal value detected : " << max << std::endl;

    //
    // ON NORMALISE TOUS LES ECHANTILLONS (UTILES)
    //
    int maxV = 1 << rBits;
    int facV = 65536 / maxV;
    std::cout << "(II) factor value for transformation : " << facV     << std::endl;
    std::cout << "(II) limit value  for transformation : " << (maxV/2) << std::endl;
    for(unsigned int q=0; q<rLength; q+=1){
        liste[q] = liste[q] / facV;
    }
    std::cout << "(II) Nombre d'echantillons       : " << liste.size() << std::endl;
    eFreq = rFreq;

    std::cout << "(II) Launchint generation" << std::endl;
    rom_classic rc(this, rBits, rFreq);
}

int frozen_bits::size( )
{
    return liste.size();
}

int frozen_bits::at(int index)
{
    return liste.at( index );
}
