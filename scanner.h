#ifndef interpres_scanner_h
#define interpres_scanner_h

#include "common.h"

typedef enum {
  TOKEN_LEFT_PARENTHESIS,
  TOKEN_RIGHT_PARENTHESIS,
  TOKEN_LEFT_BRACE,
  TOKEN_RIGHT_BRACE,
  TOKEN_DOT,
  TOKEN_COMMA,
  TOKEN_SEMICOLON,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_SLASH,
  TOKEN_STAR,
  TOKEN_BANG,
  TOKEN_BANG_EQUAL,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,
  TOKEN_IDENTIFIER,
  TOKEN_STRING,
  TOKEN_NUMBER,
  TOKEN_VAR,
  TOKEN_NIL,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_FOR,
  TOKEN_WHILE,
  TOKEN_CLASS,
  TOKEN_SUPER,
  TOKEN_THIS,
  TOKEN_FUN,
  TOKEN_PRINT,
  TOKEN_RETURN,
  TOKEN_ERROR,
  TOKEN_EOF
} TokenType;

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

typedef struct {
  TokenType type;
  const char *lexeme_start;
  size_t lexeme_length;
  size_t line_number;
} Token;

/*
 * @brief Initialize our global scanner.
 * This function will set the scanner's "lexeme_start" and "lexeme_current" to
 * the start of the source code, and set the base line number to 1 (meaning the
 * beginning of the source code input).
 *
 * @return void
 */
void init_scanner(const char *input);

/*
 * @brief Scan the next token from the source code.
 * This function will scan the next token from the source code and return it to
 * the caller.
 *
 * @return The token that was just scanned
 */
Token scan_token();

#endif
