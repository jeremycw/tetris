tetris: events.o main.o render.o tetris.o
	gcc  -g events.o main.o render.o tetris.o `sdl-config --libs` -lSDL_ttf -o tetris

main.o: main.c
	gcc -g -c main.c

tetris.o: tetris.c
	gcc -g -c tetris.c

events.o: events.c
	gcc -g -c events.c

render.o: render.c
	gcc -g -c render.c

events.h: events_h events.txt
	./events_h < events.txt

events_h: events_h.c
	gcc events_h.c -o events_h

events.o main.o render.o tetris.o: events.h tetris.h

clean:
	rm *.o tetris events_h events.h
