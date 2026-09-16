#include "../include/token.h"

TOKEN NewTOKEN(TOKEN_TYPE tYype,const char *data) {
  TOKEN newToken;
  newToken.tok_Type = tYype;
  newToken.data = (char *)malloc(strlen(data) + 1);
  strcpy(newToken.data, data);
  return newToken;
}