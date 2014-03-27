CC = gcc
LFLAGS = `sdl-config --libs` -lSDL_ttf
CLFAG = -g
OBJS = events.o main.o render.o tetris.o
tetris: $(OBJS)
	$(CC)  $(LFLAGS) $(OBJS) -o tetris

main.o: main.c
	$(CC) $(CLFAGS) -c main.c

tetris.o: tetris.c
	$(CC) $(CFLAGS) -c tetris.c

events.o: events.c
	$(CC) $(CFLAGS) -c events.c

render.o: render.c
	$(CC) $(CFLAGS) -c render.c

events.h: events_h events.txt
	./events_h < events.txt

events_h: events_h.c
	$(CC) events_h.c -o events_h

$(OBJS): events.h tetris.h

clean:
	rm *.o tetris events_h events.h
