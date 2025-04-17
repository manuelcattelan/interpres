#ifndef interpres_token_h
#define interpres_token_h

#include <stdlib.h>

#include "scanner.h"

/* This list contains all the supported tokens for the language. */
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
  TOKEN_STAR,
  TOKEN_SLASH,
  TOKEN_BANG,
  TOKEN_BANG_EQUAL,
  TOKEN_EQUAL,
  TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER,
  TOKEN_GREATER_EQUAL,
  TOKEN_LESS,
  TOKEN_LESS_EQUAL,
  TOKEN_IDENTIFIER,
  TOKEN_VAR,
  TOKEN_STRING,
  TOKEN_NUMBER,
  TOKEN_NIL,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_AND,
  TOKEN_OR,
  TOKEN_TRUE,
  TOKEN_FALSE,
  TOKEN_FUNCTION,
  TOKEN_FOR,
  TOKEN_WHILE,
  TOKEN_RETURN,
  TOKEN_CLASS,
  TOKEN_THIS,
  TOKEN_SUPER,
  TOKEN_PRINT,
  TOKEN_ERROR,
  TOKEN_EOF
} TokenType;
/* Each token that our scanner produces have a type, a lexeme (the actual
 * string that was scanned), the length of the lexeme and the line number in
 * which it was found. */
typedef struct {
  TokenType type;
  const char *lexeme_start;
  size_t lexeme_length;
  size_t line_number;
} Token;
/*
 * @brief Scan a new character from input.
 * This function will map a character from the input to a token type after
 * skipping any whitespace, newlines or comments that may be present in the
 * input.
 *
 * @param scanner A pointer to the scanner that will scan the input
 * @return The created token
 */
Token scan_token(Scanner *scanner);

#endif
