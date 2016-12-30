CC = gcc
CFLAGS = -g -Wall -I.


all:	calc format 
	
calc: 	calc.c calc.o
	gcc -g -Wall -o calc calc.o -I.

format: format.c format.o
	gcc -g -Wall -o format format.o -I. 

clean:
	rm -f calc format calc.o format.o
