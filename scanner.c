#include "scanner.h"

/* This is a global variable definition that avoids the chore of passing around
 * a pointer to the scanner instance to all of the functions inside this module.
 * Of course, everything bad that one can hear about global variables still
 * stands with this implementation. */
Scanner scanner;

void init_scanner(const char *input) {
  scanner.lexeme_start = input;
  scanner.lexeme_current = input;
  scanner.line_number = 1;
}
