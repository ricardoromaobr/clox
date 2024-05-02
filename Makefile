CFLAGS= -g -Wall

all: main

main: main.o chunk.o memory.o debug.o value.o vm.o
	gcc $(CFLAGS) ./build/main.o ./build/chunk.o ./build/memory.o ./build/debug.o ./build/value.o ./build/vm.o -o ./build/main

main.o: main.c 
	gcc $(CFLAGS) -c main.c -o ./build/main.o 

chunk.o: chunk.c 
	gcc $(CFLAGS) -c chunk.c -o ./build/chunk.o

memory.o: memory.c
	gcc $(CFLAGS) -c memory.c -o ./build/memory.o

debug.o: debug.c
	gcc $(CFLAGS) -c debug.c -o ./build/debug.o

value.o: value.c
	gcc $(CFLAGS) -c value.c -o ./build/value.o	

vm.o: vm.c
	gcc $(CFLAGS) -c vm.c -o ./build/vm.o

clean:
	rm -f ./build/*.o ./build/main