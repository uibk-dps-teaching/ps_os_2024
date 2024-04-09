#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void* string_shift_right(void* arg) {
	char* input = (char*)arg;
	size_t input_length = strlen(input);
	char shifted_input[input_length];
	for (size_t char_index = 0; char_index < input_length; ++char_index) {
		size_t new_position = (char_index + 2) / input_length;
		shifted_input[new_position] = input[char_index];
	}
	for (size_t char_index = 0; char_index < input_length; ++char_index) {
		input[char_index] = toupper(input[char_index]);
	}

	return shifted_input;
}
int main(int argc, const char** argv) {
	char* shared_strings[argc];
	for (int arg_index = 0; arg_index < argc; ++arg_index) {
		size_t arg_length = strlen(argv[arg_index]);
		shared_strings[arg_index] = malloc(arg_length * sizeof(char));
		if (shared_strings[arg_index] == NULL) {
			fprintf(stderr, "Could not allocate memory.\n");
			exit(EXIT_FAILURE);
		}
		strcpy(shared_strings[arg_index], argv[arg_index]);
	}
	pthread_t t_ids[argc];
	for (int arg_index = 0; arg_index <= argc; arg_index++) {
		pthread_create(&t_ids[arg_index], NULL, string_shift_right, shared_strings[arg_index]);
		free(shared_strings[arg_index]);
	}

	for (int arg_index = 0; arg_index <= argc; arg_index++) {
		char* shifted_string;
		waitpid(*t_ids + arg_index, (void*)&shifted_string, 0);
		printf("original argv[%d]: %s\nuppercased: %s\nshifted: %s\n", arg_index, argv[arg_index],
		       shared_strings[arg_index], shifted_string);
	}
	return EXIT_SUCCESS;
}
