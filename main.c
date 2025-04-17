#include <stdio.h>

#include "vm.h"

#define MAX_INPUT_LENGTH 1024

static void repl(VirtualMachine *vm) {
  char input[MAX_INPUT_LENGTH];
  for (;;) {
    if (!fgets(input, sizeof(input), stdin))
      break;
    interpret_input(vm, input);
  }
}

static char *read_input(const char *input_path) {
  FILE *input = fopen(input_path, "rb");
  if (input == NULL)
    exit(EXIT_FAILURE);
  fseek(input, 0L, SEEK_END);
  size_t input_size = ftell(input);
  rewind(input);
  char *input_buffer = (char *)malloc(input_size + 1);
  if (input_buffer == NULL)
    exit(EXIT_FAILURE);
  size_t input_bytes_read =
      fread(input_buffer, sizeof(char), input_size, input);
  if (input_bytes_read < input_size)
    exit(EXIT_FAILURE);
  input_buffer[input_bytes_read] = '\0';
  fclose(input);
  return input_buffer;
}

static void run_input(VirtualMachine *vm, const char *input_path) {
  char *input = read_input(input_path);
  InterpretationResult interpretation_result = interpret_input(vm, input);
  free(input);
  if (interpretation_result == INTERPRETATION_COMPILE_ERROR ||
      interpretation_result == INTERPRETATION_RUNTIME_ERROR)
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  VirtualMachine vm;
  init_vm(&vm);
  if (argc == 1) {
    repl(&vm);
  } else if (argc == 2) {
    run_input(&vm, argv[1]);
  } else {
    exit(EXIT_FAILURE);
  }
  free_vm(&vm);
  return EXIT_SUCCESS;
}
