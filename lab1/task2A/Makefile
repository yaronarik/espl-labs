encoder: encoder.o
	gcc -m32 encoder.o -o encoder
	
encoder.o : encoder.c
	gcc -g -Wall -m32 -c -o encoder.o encoder.c
	
clean:
	rm *o encoder
