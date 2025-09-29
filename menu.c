#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "sdl_window.h"

static char **detected_data;
static int detected_data_count;
static int selected_data;

void init_menu() {

	detected_data = malloc(sizeof(char *) * 100);
	detected_data_count = 0;
	selected_data = 0;

	DIR *root_dir = opendir(".");
    struct dirent *dir;

    if (!root_dir) {
		printf("Failed to open directory! Can't look for game data files!\n");
		return;
	}

	while ((dir = readdir(root_dir)) != NULL) {

		if (strstr(dir->d_name, ".txt")) {

			detected_data[detected_data_count] = malloc(sizeof(char) * (strlen(dir->d_name) + 1));

			memcpy(detected_data[detected_data_count], dir->d_name, sizeof(char) * (strlen(dir->d_name) + 1));

			detected_data_count++;
		}
	}

	closedir(root_dir);
}

void process_menu(const Input *input) {

	for (int i=0; i<detected_data_count; i++) {

		draw_text(20, 10 + i * 30, detected_data[i]);
	}

	draw_text(0, 10 + selected_data * 30, ">");
	
	if (input->up && input->up_edge) {

		selected_data--;

		if (selected_data < 0)
			selected_data = detected_data_count - 1;
	}

	if (input->down && input->down_edge) {
		
		selected_data++;

		if (selected_data >= detected_data_count)
			selected_data = 0;
	}

	if (input->select_edge)
		start_game(detected_data[selected_data]);
}

void destroy_menu() {
	
	free(detected_data);
}