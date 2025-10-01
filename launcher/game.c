#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sdl_window.h"
#include "state.h"

static State **states;
static int state_count;
static int current_state_index;

int init_game(const char *game_data_path) {

	FILE *file = fopen(game_data_path, "r");

	if (!file) {
		printf("Could not open game file!\n");
		return FALSE;
	}

	// read start state and total state count
	if (fscanf(file, "%u %u", &current_state_index, &state_count) != 2) {

		fclose(file);
		return FALSE;
	}

	// allocate space for the states
	states = malloc(sizeof(State *) * state_count);

	// read all the states
	char flag[100] = {};
	int i = 0;
	int scanned;

	while (TRUE) {

		scanned = fscanf(file, "%99s", flag);

		if (scanned == EOF) {

			fclose(file);
			return TRUE;
		}

		if (scanned != 1 || ferror(file)) {

			for (int state=0; state<i; state++)
				states[state]->destroy_state(states[state]);
			free(states);
			fclose(file);

			return FALSE;
		}

		if (strcmp(flag, "#COMMENT") == 0) {

			fscanf(file, "\"%*[^\"]\"");

		}else if (strcmp(flag, "#DIALOGUE") == 0) {

			// #DIALOGUE <successor_index>:"<message>"

			int successor_index;
			char *message = calloc(2048, sizeof(char));

			fscanf(file, "%u:\"%2047[^\"]\"", &successor_index, message);

			states[i++] = create_dialogue_state(successor_index, message);

		} else if (strcmp(flag, "#CHOICE") == 0) {

			// #CHOICE <choice_count> "<prompt>" <succ_1>:"<text_1>" <succ_2>:"<text_2>" ...

			int choice_count;
			char *prompt = calloc(2048, sizeof(char));

			fscanf(file, "%u \"%2047[^\"]\"", &choice_count, prompt);

			char **choice_texts = malloc(sizeof(char *) * choice_count);
			int *choice_successors = malloc(sizeof(int *) * choice_count);

			for (int choice = 0; choice < choice_count; choice++) {

				choice_texts[choice] = calloc(2048, sizeof(char));

				fscanf(file, "%u:\"%2047[^\"]\"", &choice_successors[choice], choice_texts[choice]);
			}

			states[i++] = create_choice_state(prompt, choice_count, choice_texts, choice_successors);
		}
	}
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