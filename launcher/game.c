#include <stdio.h>
#include <string.h>
#include "sdl_window.h"

void init_game(const char *game_data_path) {

	FILE *file = fopen(game_data_path, "r");

	if (!file) {
		printf("Could not open game file!\n");
		return;
	}

	// read start state and total state count
	unsigned int start_state, state_count;

	fscanf(file, "%u %u", &start_state, &state_count);

	// read all the states
	char flag[100] = {};

	while (fscanf(file, "%99s", flag) != EOF) {

		if (strcmp(flag, "#DIALOGUE") == 0) {

			unsigned int successor_state;
			char text[2048] = {};

			fscanf(file, "%u:\"%2047[^\"]\"", &successor_state, text);

			printf("%s\n", text);

		} else if (strcmp(flag, "#COMMENT") == 0) {

			fscanf(file, "\"%*[^\"]\"");

		}
	}


	// #DIALOGUE 1:"This is a dialogue! Please pick a choice!"

	// #CHOICE 2 2:"Eat bacon!" 3:"Eat pancakes!"

	fclose(file);
}

void process_game(const Input *input) {

}

void destroy_game() {

}