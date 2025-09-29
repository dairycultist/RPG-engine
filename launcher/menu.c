#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "sdl_window.h"

static char **detected_data;
static int detected_data_count;
static int selected;

void init_menu() {

	detected_data = malloc(sizeof(char *) * 100);
	detected_data_count = 0;
	selected = 0;

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

	draw_text(20, 0, "Quit");

	for (int i=0; i<detected_data_count; i++) {

		draw_text(20, 30 + i * 30, detected_data[i]);
	}

	draw_text(0, 30 + selected * 30, ">");
	
	if (input->up && input->up_edge) {

		if (--selected < -1)
			selected = detected_data_count - 1;
	}

	if (input->down && input->down_edge) {

		if (++selected >= detected_data_count)
			selected = -1;
	}

	if (input->select && input->select_edge) {

		if (selected == -1) {
			signal_exit();
		} else {
			signal_start_game(detected_data[selected]);
		}
	}
}

void destroy_menu() {
	
	free(detected_data);
}