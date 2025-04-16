#ifndef interpres_parser_h
#define interpres_parser_h

#include "chunk.h"
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
  PRECEDENCE_PRIMARY,
} Precedence;

typedef void (*ParseFn)();

typedef struct {
  ParseFn prefix_function;
  ParseFn infix_function;
  Precedence precedence;
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
                                 TokenType expected_token_type,
                                 const char *error_message);

/*
 * @brief Add the given constant to the chunk.
 * This function will add the given constant to the chunk's constants array
 * while also checking if the constant's index is within the limits of the
 * constants array itself.
 *
 * @param parser A pointer to the parser that will parse the input
 * @param currently_compiling_chunk A pointer to the chunk that will hold the
 * constant
 * @paramc constant The constant to add to the chunk
 * @return The index of the constant in the chunk's constants array
 */
uint8_t make_parser_constant(Parser *parser, Chunk *currently_compiling_chunk,
                             Constant constant);

void parse_expression_precedence(Parser *parser, Scanner *scanner,
                                 Precedence precedence);

void parse_expression(Parser *parser, Scanner *scanner);

#endif
