#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sdl_window.h"
#include "state.h"

static State **states;
static int state_count;
static int current_state_index;

void init_game(const char *game_data_path) {

	FILE *file = fopen(game_data_path, "r");

	if (!file) {
		printf("Could not open game file!\n");
		return;
	}

	// read start state and total state count
	fscanf(file, "%u %u", &current_state_index, &state_count);

	// allocate space for the states
	states = malloc(sizeof(State *) * state_count);

	// read all the states
	char flag[100] = {};
	int i = 0;

	while (fscanf(file, "%99s", flag) != EOF) {

		if (strcmp(flag, "#COMMENT") == 0) {

			fscanf(file, "\"%*[^\"]\"");

		}else if (strcmp(flag, "#DIALOGUE") == 0) {

			// #DIALOGUE <successor_index>:"<text>"

			int successor_index;
			char text[2048] = {};

			fscanf(file, "%u:\"%2047[^\"]\"", &successor_index, text);

			states[i++] = create_dialogue_state(successor_index, text);

		} else if (strcmp(flag, "#CHOICE") == 0) {

			// #CHOICE "<prompt>" <choice_count> <succ_1>:"<text_1>" <succ_2>:"<text_2>" ...

			char prompt[2048] = {};
			int choice_count;

			fscanf(file, "%u \"%2047[^\"]\"", &choice_count, prompt);

			char **choice_texts = malloc(sizeof(char *) * choice_count);
			int *choice_successors = malloc(sizeof(int *) * choice_count);

			for (int choice = 0; choice < choice_count; choice++) {

				choice_texts[choice] = malloc(sizeof(char) * 2048);

				fscanf(file, "%u:\"%2047[^\"]\"", &choice_successors[choice], choice_texts[choice]);
			}

			states[i++] = create_choice_state(prompt, choice_count, choice_texts, choice_successors);
		}
	}

	fclose(file);
}

void process_game(const Input *input) {

	int next_state_index = states[current_state_index]->process_state(states[current_state_index], input);

	if (next_state_index != -1)
		current_state_index = next_state_index;
}

void destroy_game() {

	for (int i=0; i<state_count; i++)
		states[i]->destroy_state(states[i]);

	free(states);
}