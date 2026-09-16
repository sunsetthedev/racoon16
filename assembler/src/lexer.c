#include "../include/lexer.h"
#include "../include/token.h"
#include <string.h>

void LexerAddToken(Lexer *lexer, TOKEN token) {
  lexer->tokens = (TOKEN *)realloc(lexer->tokens, sizeof(TOKEN) * (lexer->tok_count + 1));
  memcpy(&lexer->tokens[lexer->tok_count], &token, sizeof(token));
  lexer->tok_count++;
}


Lexer *NewLexer(const char *filename) {
  Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
  lexer->tokens = NULL;
  lexer->tok_count = 0;
  return lexer;
}