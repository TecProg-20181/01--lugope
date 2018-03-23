all: Photopobre.exe

Photopobre.exe: main.o ppmlib.o
	gcc -o Photopobre.exe main.o ppmlib.o

ppmlib.o: ppmlib.c ppmlib.h
	gcc -o ppmlib.o -c ppmlib.c -W -Wall -Wshadow -pedantic

main.o: main.c ppmlib.h
	gcc -o main.o -c main.c -W -Wall -Wshadow -pedantic

.PHONY: clean 

clean: 
	rm ./Photopobre.exe
	rm  ./*.o