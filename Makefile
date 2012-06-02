tetris: events.o main.o render.o tetris.o
	gcc  -g events.o main.o render.o tetris.o -lSDL -lSDL_ttf -o tetris

main.o: main.c
	gcc -g -c main.c

tetris.o: tetris.c
	gcc -g -c tetris.c

events.o: events.c
	gcc -g -c events.c

render.o: render.c
	gcc -g -c render.c

events.o main.o render.o tetris.o: events.h tetris.h
