all: clean ia gui

gui:
	-gcc -Wall -o backgammon src/gui/*.c `sdl-config --cflags --libs`

ia:
	-gcc -shared -Wall -o ia.so src/ia/*.c

clean:
	-rm backgammon ia.so

