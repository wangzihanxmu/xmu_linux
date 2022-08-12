wzh: main.o my_ipc.o hotel.o
	g++ -o wzh main.o my_ipc.o hotel.o
	
main.o: main.c my_ipc.h hotel.h
	g++ -c main.c my_ipc.h hotel.h
	
my_ipc.o: my_ipc.c my_ipc.h
	g++ -c my_ipc.c my_ipc.h

hotel.o: hotel.c hotel.h	
	g++ -c hotel.c hotel.h	
	
.PHONY : clean
clean :
	-rm wzh main.o my_ipc.o hotel.o my_ipc.h.gch hotel.h.gch
