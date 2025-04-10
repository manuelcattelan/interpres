#ifndef interpres_scanner_h
#define interpres_scanner_h

#include "common.h"

/* Our scanner object keeps track of how far it has gone through the user's
 * source code by using a counter, "line_number", which tracks what line the
 * current lexeme is on, and two pointers, "lexeme_start" and "lexeme_current",
 * which point respectively to the start of the current lexeme being scanned and
 * the current character being looked at. */
typedef struct {
  const char *lexeme_start;
  const char *lexeme_current;
  size_t line_number;
} Scanner;

/*
 * @brief Initialize our global scanner.
 * This function will set the scanner's "lexeme_start" and "lexeme_current" to
 * the start of the source code, and set the base line number to 1 (meaning the
 * beginning of the source code input).
 *
 * @return void
 */
void init_scanner(const char *input);

#endif
