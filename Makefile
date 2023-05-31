make: mainfp.o dsa.o 
	gcc mainfp.o dsa.o -lm
	./a.out

mainfp.o: 
	gcc -c mainfp.c 

dsa.o: 
	gcc -c  dsa.c


