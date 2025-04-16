#ifndef interpres_scanner_h
#define interpres_scanner_h

#include <stdbool.h>
#include <stdlib.h>

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
 * @brief Initialize the scanner.
 * In its main call, this function will set the scanner's "lexeme_start" and
 * "lexeme_current" to the start of the source code, and set the base line
 * number to 1 (meaning the beginning of the source code input).
 *
 * @param scanner A pointer to the scanner to initialize
 * @param input The input set of instructions to scan
 * @param line_number The line number of the first instruction to scan
 * @return void
 */
void init_scanner(Scanner *scanner, const char *input, size_t line_number);

/*
 * @brief Determine if the scanner has reached the end of the input.
 * This function will determine if the scanner has reached the end of the input
 * by controlling if the character pointed to by the "lexeme_current" pointer is
 * equal to the null terminator '\0'.
 *
 * @param scanner A pointer to the scanner to check
 * @return Whether the scanner has reached the end of the input or not
 */
bool scanner_is_eof(Scanner *scanner);

/*
 * @brief Advance the scanner to the next character.
 * This function will advance the scanner to the next character by moving the
 * "lexeme_current" pointer to the next character in the input string, returning
 * the character that was just read.
 *
 * @param scanner A pointer to the scanner to advance
 * @return The character that was just read
 */
char advance_scanner(Scanner *scanner);

/*
 * @brief Peek the current character without advancing the scanner.
 * This function will return the current character without advancing the scanner
 * by returning the character pointed to by the "lexeme_current" pointer.
 *
 * @param scanner A pointer to the scanner to advance
 * @return The character that was just read
 */
char peek_scanner(Scanner *scanner);

/*
 * @brief Peek the character next to the current character without advancing the
 * scanner.
 * This function will return the character next to the current character without
 * advancing the scanner by returning the character pointed to by the
 * "lexeme_current" pointer + 1.
 *
 * @param scanner A pointer to the scanner to advance
 * @return The character that was just read
 */
char peek_scanner_next(Scanner *scanner);

#endif
