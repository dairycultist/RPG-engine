#ifndef STATE_H
#define STATE_H

typedef struct {

	// all states start with these two functions (pseudo-inheritence through composition)
	int (*process_state)(void *state, const Input *input); // returns next state, or -1 for no change
	void (*destroy_state)(void *state);

} State;

State *create_dialogue_state(int successor_index, const char *text);
State *create_choice_state(const char *text, int choice_count, char **choice_texts, int *choice_successors);

#endif