project: main.o fp.o
	gcc main.o fp.o -lm -o project
main.o: main.c fp.h
	gcc -c  main.c	
fp.o: fp.c fp.h
	gcc -c  fp.c
