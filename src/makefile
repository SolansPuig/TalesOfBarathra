CC = gcc
CFLAGS = -g
LIBS = -lSDL2 -lSDL2_image -lm
OBJ = graphics.o sprite.o game_loop.o entity.o world.o math.o frozen.o
DEBUG = -g

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

tob: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f *.o *.out
