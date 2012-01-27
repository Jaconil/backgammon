all: clean ia gui

gui:
	-gcc -Wall -o backgammon src/gui/*.c -lSDL -lSDL_image -lSDL_ttf

ia:
	-gcc -shared -Wall -fPIC -o ia.so src/ia/*.c

clean:
	-rm backgammon ia.so

