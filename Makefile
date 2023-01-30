CC := gcc
CFLAGS := -I src/include
LIBS := -L src/lib -lmingw32 -lSDL2main -lSDL2
RM := del

OBJS = main.o rendering.o logic.o

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

*.o: *.c
	$(CC) $(CFLAGS) -c $^

clean:
	$(RM) *.o main.exe