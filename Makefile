.PHONY: launcher clean

launcher/app: launcher/*.c
	gcc -o launcher/app $^ -lSDL2_image -lSDL2_ttf $(shell sdl2-config --cflags) $(shell sdl2-config --libs)

launcher: launcher/app
	./launcher/app

clean:
	rm launcher/app