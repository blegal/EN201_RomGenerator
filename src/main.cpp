
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <sstream>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "./vhdl/frozen_bits.h"

#include <iomanip>
#include <iostream>
#include <math.h>

#define abs(x) ((x<0?(-x):(x)))

////////////////////////////////////////////////////////////
/// Play a sound
///
////////////////////////////////////////////////////////////

#include "filters/Copy.h"
#include "filters/noise/NoiseAWNG.h"

int main(int argc, char* argv[])
{
    std::cout << "(II) A little program developped by B. LE GAL for teaching purpose (" << __DATE__ << " " << __TIME__ << ")" << endl;
    std::cout << "(II) In converts a WAV/MP3/OGG file to a VHDL ROM for FPGA implementation" << endl;

    std::string filename = "./resources/canary.wav";
    if (argc >= 2)
    {
        filename = argv[1];
    }

    unsigned int frequence =  44100; // 11025, 22050, 44100
    unsigned int nb_bits   =     11;
    unsigned int length    = 262144;
    bool noise             =  false;

    for(int i=2; i<argc; i++)
    {
      if( strcmp(argv[i-1], "-output") == 0 ){
        std::cout << "(II) Modifying frequency parameter : " << frequence << endl;

      }else if( strcmp(argv[i-1], "-frequency") == 0 ){
        frequence = atoi( argv[i] );
        i += 1;
        std::cout << "(II) Modifying frequency parameter : " << frequence << endl;

      }else if( strcmp(argv[i-1], "-bits") == 0 ){
        nb_bits = atoi( argv[i] );
        i += 1;
        std::cout << "(II) Modifying # bits parameter : " << nb_bits << endl;

      }else if( strcmp(argv[i-1], "-length") == 0 ){
        length = atoi( argv[i] );
        i += 1;
        std::cout << "(II) Modifying length parameter : " << length << endl;

      }
    }

    sf::SoundBuffer buffer1;
    if (!buffer1.loadFromFile( filename ))
        return 1;

    sf::SoundBuffer buffer2;
    if (!buffer2.loadFromFile( filename ))
        return 1;

    //
    // ON CHARGE LE FICHIER AUDIO DANS LES 2 BUFFERS
    //
    RawSound buff_1( &buffer1 );
    RawSound buff_2( &buffer2 );

    std::cout << "#################################################" << endl;
    std::cout << "(II) Audio filename        : " << filename << endl;
    std::cout << "(II) Audio file frequency  : " << buff_1.sample_rate() << endl;
    std::cout << "(II) Audio file channels   : " << buff_1.channels()    << endl;
    std::cout << "(II) Audio file length     : " << buff_1.length()      << endl;
    std::cout << "(II) Audio file length (s) : " << ((float)buff_1.length() / (float)buff_1.sample_rate()) << endl;
    std::cout << "#################################################" << endl;
    std::cout << "(II) Target frequency      : " << frequence << endl;
    std::cout << "(II) Target channels       : " << 1    << endl;
    std::cout << "(II) Target length         : " << length      << endl;
    std::cout << "(II) Target length (s)     : " << ((float)length / (float)frequence) << endl;
    std::cout << "#################################################" << endl;

    length = (length>buff_1.length()) ? buff_1.length() : length;

    //
    // ON CREE UN OBJET DE RECOPIE QUI POURRA ETRE UTILE
    // PAR LA SUITE...
    //
    Copy soundCopy;

    //
    // ON AJOUTE DU BRUIT DANS LE SIGNAL
    //
    if( noise == true ){
        NoiseAWNG noise;
        noise.process    (&buff_1, &buff_2);
        soundCopy.process(&buff_2, &buff_1);
    }

    //
    //
    //
    if( buff_1.sample_rate() < frequence ){
        std::cout << "(EE) The audio file has a frequency lower than the targeted frequency." << endl;
        std::cout << "(EE) Upsampling is not supported yet by this software." << endl;
        std::cout << "(EE) WAV frequency = " << buff_1.sample_rate() << endl;
        std::cout << "(EE) ASK frequency = " << frequence            << endl;
        exit( 0 );
    }

/*
    //
    //
    //
    unsigned int subsampling = (buff_1.sample_rate() / frequence);
    unsigned int real_length = buff_1.length() / subsampling;
    if( real_length < length ){
        std::cout << "(EE) The audio file has a frequency lower than the targeted frequency." << endl;
        std::cout << "(EE) Upsampling is not supported yet by this software." << endl;
        std::cout << "(EE) WAV length = " << buff_1.length() << endl;
        std::cout << "(EE) ASK length = " << length          << endl;
        std::cout << "(EE) SUB length = " << real_length     << endl;
        exit( 0 );
    }
*/
    frozen_bits sData(&buff_1, length, frequence, buff_1.channels(), nb_bits);

    return EXIT_SUCCESS;
}
