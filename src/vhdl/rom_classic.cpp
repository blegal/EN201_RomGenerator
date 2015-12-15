#include "rom_classic.h"


/**
 *
 *
 *
 */
rom_classic::rom_classic(frozen_bits* _fbits, int _nBits, int _frequency)
{
    fbits = _fbits;
    nBits = _nBits;
    frequency = _frequency;
    run();
}


/**
 *
 *
 *
 */
void rom_classic::run( )
{
    int log2v = 11; // les donnees audio sont codees sur 12 bits pour le moment
    yPos = 0; // Pour la generation de la rom
    char filename[1024];
    sprintf(filename, "rom_sound_%dHz_%d_bits.vhd", frequency, log2v);
    ofstream ofile( filename );
    if( !ofile.is_open() ){
        cout << "> Erreur à l'ouverture du fichier (" << filename << ")" << endl;
        exit( 0 );
    }

    int depth = ceil(log2( fbits->size() ));
    ofile << "LIBRARY ieee;" << endl;
    ofile << "USE ieee.std_logic_1164.ALL;" << endl;
    ofile << "USE IEEE.math_real.ALL;" << endl;
    ofile << "" << endl;
    ofile << "PACKAGE audio_rom IS" << endl;
    ofile << "	CONSTANT ROM_FREQ      : integer := "    << frequency         << ";" << endl;
    ofile << "	CONSTANT ROM_MAX_ADDR  : integer := "    << (fbits->size()) << ";" << endl;
    ofile << "	CONSTANT ROM_MAX_DATA  : integer :=  "   << (1<<(log2v))    << ";" << endl;
    ofile << "	CONSTANT ROM_ADDR_BITS : integer :=    " << depth             << ";" << endl;
    ofile << "	CONSTANT ROM_DATA_BITS : integer :=    " << log2v             << ";" << endl;
    ofile << "END;" << endl;
    ofile << "" << endl;
    ofile << "-------------------------------------------------------------------------" << endl;
    ofile << "" << endl;
    ofile << "LIBRARY IEEE;" << endl;
    ofile << "USE IEEE.STD_LOGIC_1164.ALL;" << endl;
    ofile << "USE IEEE.NUMERIC_STD.ALL;" << endl;
    ofile << "USE work.audio_rom.ALL;" << endl;

    ofile << "" << endl;
    ofile << "ENTITY wav_rom IS" << endl;
    ofile << "PORT (" << endl;
//    ofile << "      RESET     : IN  STD_LOGIC;" << endl;
    ofile << "      CLOCK     : IN  STD_LOGIC;" << endl;
    ofile << "      ADDR      : IN  STD_LOGIC_VECTOR(ROM_ADDR_BITS-1 DOWNTO 0);" << endl;
    ofile << "      DATA_OUT  : OUT STD_LOGIC_VECTOR(ROM_DATA_BITS-1 DOWNTO 0)" << endl;
    ofile << "      );" << endl;
    ofile << "END wav_rom;" << endl;
    ofile << "" << endl;
    ofile << "ARCHITECTURE Behavioral OF wav_rom IS" << endl;
    ofile << "  TYPE     rom_type IS ARRAY (0 TO (ROM_MAX_ADDR-1)) OF SIGNED (ROM_DATA_BITS-1 DOWNTO 0);" << endl;
    ofile << "  CONSTANT memory : rom_type := (" << endl;
    for(int q=0; q<fbits->size(); q++){
      char buffer[1024];
        if( q == (fbits->size()-1) ) {
          sprintf(buffer, " TO_SIGNED(%+5d, %d) -- %d", fbits->at(q), nBits, q);
          ofile << buffer << endl;
        }else{
          sprintf(buffer, " TO_SIGNED(%+5d, %d), -- %d", fbits->at(q), nBits, q);
          ofile << buffer << endl;
//            ofile << "    TO_SIGNED(" << fbits->at(q) << ", " << nBits << "),";
            if( (q+1)%8 == 0 ) ofile << endl;
        }
    }
    ofile << "  );" << endl;
    ofile << "" << endl;
    ofile << "BEGIN" << endl;
    ofile << "" << endl;
    ofile << "  PROCESS (CLOCK)" << endl;
    ofile << "  BEGIN" << endl;
    ofile << "    IF (CLOCK'event AND CLOCK = '1') THEN" << endl;
    ofile << "      DATA_OUT <= STD_LOGIC_VECTOR( memory(to_integer(UNSIGNED(ADDR))) );" << endl;
    //ofile << "      DATA_OUT <= STD_LOGIC_VECTOR(RESIZE(memory(to_integer(UNSIGNED(CPT_R))), 11 );" << endl;
    ofile << "    END IF;" << endl;
    ofile << "  END PROCESS;" << endl;
    ofile << "" << endl;
    ofile << "END Behavioral;" << endl;
    ofile.close();
}
