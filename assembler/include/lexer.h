#include "token.h"
typedef struct
{
  TOKEN *tokens;
  int tok_count;

}Lexer;

Lexer *NewLexer(const char *filename);
void LexerAddToken(Lexer *lexer, TOKEN token);
