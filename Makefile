CFLAGS= -g -Wall

all: main

main: main.o chunk.o memory.o debug.o value.o vm.o compiler.o scanner.o object.o table.o
	gcc $(CFLAGS) ./build/main.o ./build/chunk.o  \
					./build/memory.o ./build/debug.o \
					./build/value.o ./build/vm.o \
					./build/scanner.o ./build/compiler.o \
					./build/object.o \
					./build/table.o \
					-o ./build/main

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

scanner.o: scanner.c
	gcc $(CFLAGS) -c scanner.c -o ./build/scanner.o

compiler.o: compiler.c
	gcc $(CFLAGS) -c compiler.c -o ./build/compiler.o

object.o: object.c
	gcc $(CFLAGS) -c object.c -o ./build/object.o

table.o: table.c
	gcc $(CFLAGS) -c table.c -o ./build/table.o

clean:
	rm -f ./build/*.o ./build/main