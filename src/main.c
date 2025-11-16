#include "token.h"
#include "parser.h"

#if defined(_WIN64) || defined(_WIN32)
#define _WIN
#endif

#ifdef _WIN
#include <windows.h>
#endif


int main(int argc, char* argv[])
{
    FILE   *fptr    =   NULL;
    Token  *token   =   NULL;
    AST    *ast     =   NULL;

#ifdef _WIN
    SetConsoleOutputCP(CP_UTF8);
#endif

    if (argc != 2)
    {
        goto error;
    }
    
    fptr    =   fopen(argv[1], "rb");
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
