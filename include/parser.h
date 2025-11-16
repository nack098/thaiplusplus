#pragma once
#include "./token.h"

typedef struct AST AST;
void    ast_free(AST**);

AST    *parser(Token *token);

struct AST
{
};
