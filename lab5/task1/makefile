all:  start

start: start.o
	gcc -m32 -g -Wall -o start start.o

start.o: start.s 
	nasm -g -f elf -w+all -o start.o start.s 

.PHONY: clean  

clean:
	rm -f *.o start