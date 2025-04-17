#ifndef interpres_parser_h
#define interpres_parser_h

#include "scanner.h"

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

typedef void (*ParseFunction)();

typedef struct {
  ParseFunction prefix_function;
  ParseFunction infix_function;
  ParsePrecedence precedence;
} ParseRule;

#endif
