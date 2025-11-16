#include "token.h"
#include "parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
    FILE   *fptr    =   NULL;
    Token  *token   =   NULL;
    AST    *ast     =   NULL;

    if (argc != 2)
    {
        goto error;
    }
    
    fptr    =   fopen(argv[1], "r");
    token   =   tokenizer(fptr);
    ast     =   parser(token);

    print_token(token);

    fclose(fptr);
    ast_free(&ast);
    token_free(&token);

    return 0;
error:
    return 1;
}
