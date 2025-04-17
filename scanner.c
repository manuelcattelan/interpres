#include <stdbool.h>

#include "scanner.h"

void init_scanner(Scanner *scanner, const char *input) {
  scanner->lexeme_start = input;
  scanner->lexeme_current = input;
  scanner->line_number = 1;
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

bool match_current_scanner_token(Scanner *scanner, char expected_token) {
  if (scanner_is_eof(scanner) || *scanner->lexeme_current != expected_token)
    return false;
  scanner->lexeme_current++;
  return true;
}
