#include "parser.h"
#include <stdlib.h>

AST *parser(Token *token)
{
    return NULL;
}

void ast_free(AST **ast)
{
    if (ast == NULL || *ast == NULL) return;

    free(ast);
}
