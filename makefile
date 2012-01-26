all: clean ia gui

gui:
	-gcc -Wall -g -o backgammon src/gui/*.c -lSDL -lSDL_image -lSDL_ttf

ia:
	-gcc -shared -Wall -o ia.so src/ia/*.c

clean:
	-rm backgammon ia.so

