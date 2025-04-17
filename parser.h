#ifndef interpres_parser_h
#define interpres_parser_h

#include "chunk.h"
#include "scanner.h"
#include "token.h"

/* Our parser keeps asking the scanner for the next token and stores it for
 * later use. Before doing that, it takes the "old" current token and stashes
 * it in the "previous_token" field. This way, we can keep track of the last
 * token we read and the current one. */
typedef struct {
  Token current_token;
  Token previous_token;
  bool is_error;
  bool is_panic;
} Parser;
/* The definition of predecence is intrinsic in the definition of the below
 * enum, since C implicitly gives successively larger numbers for enum
 * properties. */
typedef enum {
  PRECEDENCE_NONE,
  PRECEDENCE_ASSIGNMENT,
  PRECEDENCE_OR,
  PRECEDENCE_AND,
  PRECEDENCE_EQUALITY,
  PRECEDENCE_COMPARISON,
  PRECEDENCE_TERM,
  PRECEDENCE_FACTOR,
  PRECEDENCE_UNARY,
  PRECEDENCE_CALL,
  PRECEDENCE_PRIMARY
} ParsePrecedence;
/* This type defines the function signature for a parsing function. Each
 * parsing function will take a pointer to the parser, a pointer to the
 * scanner and a pointer to the chunk that we're currently compiling. The
 * function will then parse the token and generate the corresponding bytecode
 * instruction. */
typedef void (*ParseFunction)(Parser *parser, Scanner *scanner,
                              Chunk *currently_compiling_chunk);
/* A parsing rule is made of two parsing functions: a prefix function and an
 * infix function. The prefix function is used to parse the token when it is
 * the first token in an expression, while the infix function is used to parse
 * the token when it is the second token in an expression. The precedence of
 * the token is also stored in the parsing rule. */
typedef struct {
  ParseFunction prefix_function;
  ParseFunction infix_function;
  ParsePrecedence precedence;
} ParseRule;
/*
 * @brief Initialize the parser.
 * This function will set the parser's "is_error" and "is_panic" fields to
 * false, resetting in fact the parser to a non-error state.
 *
 * @param parser A pointer to the parser to initialize
 * @return void
 */
void init_parser(Parser *parser);
/*
 * @brief Advance the parser to the next token.
 * This function will advance the parser to the next token by calling the
 * scanner's scan_token function and storing the result in the parser's
 * "current_token" field. It also handles any errors that may occur during
 * scanning.
 *
 * @param parser A pointer to the parser to advance
 * @param scanner A pointer to the scanner that will scan the input
 * @return void
 */
void advance_parser(Parser *parser, Scanner *scanner);
/*
 * @brief Advance the parser and validate the current token.
 * This function will validate the current token to make sure it is of the
 * expected type and, if it is, advance the parser forward.
 *
 * @param parser A pointer to the parser to advance
 * @param scanner A pointer to the scanner that will scan the input
 * @param expected_token_type The expected token type
 * @param error_message The message to return if the token type is incorrect
 * @return void
 */
void advance_and_validate_parser(Parser *parser, Scanner *scanner,
                                 TokenType token_type,
                                 const char *error_message);
/*
 * @brief Print error message to stderr.
 * This function will print the error message to stderr, along with the line
 * number and the token that caused the error. It will also set the parser's
 * "is_error" and "is_panic" fields to true, indicating that an error has
 * occurred.
 *
 * @param parser A pointer to the parser to print the error message for
 * @param token A pointer to the token that caused the error
 * @param error_message The error message to print
 * @return void
 */
void parser_error_at(Parser *parser, Token *token, const char *error_message);
/*
 * @brief Print error message to stderr.
 * This function is a wrapper around parser_error_at, which will print the error
 * message for the previously scanned token.
 *
 * @param parser A pointer to the parser to print the error message for
 * @param error_message The error message to print
 * @return void
 */
void parser_error_at_previous(Parser *parser, const char *error_message);
/*
 * @brief Print error message to stderr.
 * This function is a wrapper around parser_error_at, which will print the error
 * message for the currently scanned token.
 *
 * @param parser A pointer to the parser to print the error message for
 * @param error_message The error message to print
 * @return void
 */
void parser_error_at_current(Parser *parser, const char *error_message);

#endif
