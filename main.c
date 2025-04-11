#include "vm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void repl() {
  char input[1024];
  for (;;) {
    printf("> ");
    if (!fgets(input, sizeof(input), stdin)) {
      printf("\n");
      break;
    }
    interpret(input);
  }
}

static char *read_file(const char *file_path) {
  FILE *file = fopen(file_path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\".\n", file_path);
    exit(EXIT_FAILURE);
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char *file_buffer = (char *)malloc(file_size + 1);
  if (file_buffer == NULL) {
    fprintf(stderr, "Not enough memory to read file \"%s\".\n", file_path);
    exit(EXIT_FAILURE);
  }

  size_t file_bytes_read = fread(file_buffer, sizeof(char), file_size, file);
  if (file_bytes_read < file_size) {
    fprintf(stderr, "Could not read file \"%s\".\n", file_path);
    exit(EXIT_FAILURE);
  }

  file_buffer[file_bytes_read] = '\0';

  fclose(file);
  return file_buffer;
}

static void run_file(const char *file_path) {
  char *input = read_file(file_path);
  InterpretationResult interpretation_result = interpret(input);
  free(input);

  if (interpretation_result == INTERPRETATION_COMPILE_ERROR ||
      interpretation_result == INTERPRETATION_RUNTIME_ERROR)
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  init_vm();
  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    run_file(argv[1]);
  } else {
    fprintf(stderr, "Usage: interpres [file_path]\n");
    exit(EXIT_FAILURE);
  }
  free_vm();
  return EXIT_SUCCESS;
}
