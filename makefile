all: clean ia gui

gui:
	-gcc -o backgammon src/gui/*.c `sdl-config --cflags --libs`

ia:
	-gcc -shared -o ia.so src/ia/*.c

clean:
	-rm backgammon ia.so

