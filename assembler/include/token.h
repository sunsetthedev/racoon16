typedef enum {
  MOV,
  ADD,
  REGS,
  EOF,
  HLT,
  COMMA,
  SEMICOLON,
  LABEL,
  NUMBER,
  STRING,
  UNKNOWN
}TOKEN_TYPE;

typedef struct {
  TOKEN_TYPE tok_Type;
  char *data;
} TOKEN;

TOKEN NewTOKEN(TOKEN_TYPE tYype,const char *data);
