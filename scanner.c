#include "scanner.h"

void init_scanner(Scanner *scanner, const char *input, size_t line_number) {
  scanner->lexeme_start = input;
  scanner->lexeme_current = input;
  scanner->line_number = line_number;
}

bool scanner_is_eof(Scanner *scanner) {
  return *scanner->lexeme_current == '\0';
}

char advance_scanner(Scanner *scanner) {
  scanner->lexeme_current++;
  return scanner->lexeme_current[-1];
}

char peek_scanner(Scanner *scanner) { return *scanner->lexeme_current; }

char peek_scanner_next(Scanner *scanner) {
  if (scanner_is_eof(scanner))
    return '\0';
  return scanner->lexeme_current[1];
}
