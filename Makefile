CC=g++
#CFLAGS=-m32 -O2 -Wall
#LDFLAGS=-m32 --stdlib=libc++ -L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
CFLAGS=-O2 -Wall
LDFLAGS=-L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
EXEC=main
SRC=./src/filters/Filter.cpp ./src/vhdl/frozen_bits.cpp ./src/vhdl/rom_classic.cpp  ./src/main.cpp ./src/RawSound.cpp  ./src/filters/Copy.cpp ./src/filters/noise/NoiseAWNG.cpp

OBJ= $(SRC:.cpp=.o)

CFLAGS=

all: $(EXEC)
	./bin/main

main: $(OBJ)
	$(CC) $(CFLAGS) -o ./bin/$@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean: $(OBJ)
	find ./src/ -name "*.o" -exec rm {} \;

mrproper: clean
	rm $(EXEC)
