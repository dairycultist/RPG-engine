.PHONY: run clean

engine: sdl_window.c game.c menu.c
	gcc -o engine sdl_window.c game.c menu.c -lSDL2_image -lSDL2_ttf $(shell sdl2-config --cflags) $(shell sdl2-config --libs)

run: engine
	./engine

clean:
	rm engine