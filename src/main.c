#include "parser.h"
#include "token.h"

#if defined(_WIN64) || defined(_WIN32)
    #define _WIN
#endif

#ifdef _WIN
    #include <windows.h>
#endif

int main(int argc, char *argv[])
{
    FILE *fptr = NULL;
    Token *token = NULL;
    AST *ast = NULL;
    int err = 0;

#ifdef _WIN
    SetConsoleOutputCP(CP_UTF8);
#endif

    if (argc != 2)
    {
        goto error;
    }

    err = fopen_s(&fptr, argv[1], "rb");
    if (err != 0)
    {
        goto error;
    }

    token = tokenizer(fptr);
    ast = parser(token);

    fclose(fptr);
    ast_free(&ast);
    token_free(&token);

    return 0;
error:
    return 1;
}
