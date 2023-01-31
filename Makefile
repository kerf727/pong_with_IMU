CC = gcc
COMPILER_FLAGS = 
INCLUDE_PATHS = -I src/include
# INCLUDE_PATHS = -I C:\Development\SDL2\x86_64-w64-mingw32\include\SDL2
LIBRARY_PATHS = -L src/lib
# LIBRARY_PATHS = -L C:\Development\SDL2\x86_64-w64-mingw32\lib
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
RM = del

OBJS = main.c rendering.c logic.c
# OBJS = main.o rendering.o logic.o

all: main

main: $(OBJS)
	$(CC) $^ $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $@

# *.o: *.c
# 	$(CC) $(CFLAGS) -c $^

clean:
	$(RM) *.o main.exe