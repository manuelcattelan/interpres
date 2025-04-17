#include <stdbool.h>
#include <stdio.h>

#include "compiler.h"
#include "parser.h"
#include "scanner.h"

Parser parser;

Chunk *compilingChunk;

static Chunk *currentChunk() { return compilingChunk; }

static void errorAt(Token *token, const char *message) {
  if (parser.is_panic)
    return;
  parser.is_panic = true;
  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
    // Nothing.
  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.is_error = true;
}

static void error(const char *message) {
  errorAt(&parser.previous_token, message);
}

static void errorAtCurrent(const char *message) {
  errorAt(&parser.current_token, message);
}

static void advance() {
  parser.previous_token = parser.current_token;

  for (;;) {
    parser.current_token = scanToken();
    if (parser.current_token.type != TOKEN_ERROR)
      break;

    errorAtCurrent(parser.current_token.start);
  }
}

static void consume(TokenType type, const char *message) {
  if (parser.current_token.type == type) {
    advance();
    return;
  }

  errorAtCurrent(message);
}

static void emitByte(uint8_t byte) {
  push_instruction_to_chunk(currentChunk(), byte, parser.previous_token.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
  emitByte(byte1);
  emitByte(byte2);
}

static void emitReturn() { emitByte(OP_RETURN); }

static uint8_t makeConstant(Constant value) {
  int constant = push_constant_to_chunk(currentChunk(), value);
  if (constant > UINT8_MAX) {
    error("Too many constants in one chunk.");
    return 0;
  }

  return (uint8_t)constant;
}

static void emitConstant(Constant value) {
  emitBytes(OP_CONSTANT, makeConstant(value));
}

static void endCompiler() { emitReturn(); }

static void expression();
static ParseRule *getRule(TokenType type);
static void parsePrecedence(ParsePrecedence precedence);

static void binary() {
  TokenType operatorType = parser.previous_token.type;
  ParseRule *rule = getRule(operatorType);
  parsePrecedence((ParsePrecedence)(rule->precedence + 1));

  switch (operatorType) {
  case TOKEN_PLUS:
    emitByte(OP_ADD);
    break;
  case TOKEN_MINUS:
    emitByte(OP_SUBTRACT);
    break;
  case TOKEN_STAR:
    emitByte(OP_MULTIPLY);
    break;
  case TOKEN_SLASH:
    emitByte(OP_DIVIDE);
    break;
  default:
    return; // Unreachable.
  }
}

static void grouping() {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

static void number() {
  double value = strtod(parser.previous_token.start, NULL);
  emitConstant(value);
}

static void unary() {
  TokenType operatorType = parser.previous_token.type;

  // Compile the operand.
  parsePrecedence(PRECEDENCE_UNARY);

  // Emit the operator instruction.
  switch (operatorType) {
  case TOKEN_MINUS:
    emitByte(OP_NEGATE);
    break;
  default:
    return; // Unreachable.
  }
}

ParseRule rules[] = {
    [TOKEN_LEFT_PAREN] = {grouping, NULL, PRECEDENCE_NONE},
    [TOKEN_RIGHT_PAREN] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_DOT] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_MINUS] = {unary, binary, PRECEDENCE_TERM},
    [TOKEN_PLUS] = {NULL, binary, PRECEDENCE_TERM},
    [TOKEN_SEMICOLON] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_SLASH] = {NULL, binary, PRECEDENCE_FACTOR},
    [TOKEN_STAR] = {NULL, binary, PRECEDENCE_FACTOR},
    [TOKEN_BANG] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_BANG_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_GREATER] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LESS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_LESS_EQUAL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_IDENTIFIER] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_STRING] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_NUMBER] = {number, NULL, PRECEDENCE_NONE},
    [TOKEN_AND] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_CLASS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_ELSE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_FALSE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_FOR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_FUN] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_IF] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_NIL] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_OR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_PRINT] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_RETURN] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_SUPER] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_THIS] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_TRUE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_VAR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_WHILE] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_ERROR] = {NULL, NULL, PRECEDENCE_NONE},
    [TOKEN_EOF] = {NULL, NULL, PRECEDENCE_NONE},
};

static void parsePrecedence(ParsePrecedence precedence) {
  advance();
  ParseFunction prefixRule =
      getRule(parser.previous_token.type)->prefix_function;
  if (prefixRule == NULL) {
    error("Expect expression.");
    return;
  }

  prefixRule();

  while (precedence <= getRule(parser.current_token.type)->precedence) {
    advance();
    ParseFunction infixRule =
        getRule(parser.previous_token.type)->infix_function;
    infixRule();
  }
}

static ParseRule *getRule(TokenType type) { return &rules[type]; }

static void expression() { parsePrecedence(PRECEDENCE_ASSIGNMENT); }

bool compile_input(const char *input, Chunk *chunk) {
  initScanner(input);
  compilingChunk = chunk;

  parser.is_error = false;
  parser.is_panic = false;

  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression.");
  endCompiler();
  return !parser.is_error;
}
