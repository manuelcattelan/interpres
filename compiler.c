#include "compiler.h"
#include "scanner.h"
#include <stdio.h>

void compile(const char *input) {
  init_scanner(input);

  int line = -1;
  for (;;) {
    Token current_token = scan_token();
    if (current_token.line_number != line) {
      printf("%4zu ", current_token.line_number);
      line = current_token.line_number;
    } else {
      printf("   | ");
    }
    printf("%2d '%.*s'\n", current_token.type, (int)current_token.lexeme_length,
           current_token.lexeme_start);
    if (current_token.type == TOKEN_EOF)
      break;
  }
}
