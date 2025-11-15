#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PARSER_BUFFER_SIZE 256

typedef struct TokenNode TokenNode;
TokenNode  *token_node_create();

typedef struct Token Token;
Token      *token_create();
void        token_append_node(Token*, TokenNode*);
void        token_free(Token**);

typedef struct AST AST;
void        ast_free(AST**);

typedef enum
{
    T_FUNC,
    T_TYPE,
    T_OPT,
    T_UNKNOWN
} ETokenType;

typedef enum 
{
    F_PARAM,
    F_NO_PARAM,
    F_UNKNOWN,
} EFunctionType;

typedef enum
{
    TT_INT,
    TT_UNKNOWN,
} EType;

typedef enum
{
    O_RETURN,
    O_UNKNOWN,
} EOperator;

struct Token
{
    TokenNode  *head;
    TokenNode  *last;
    size_t      size;
};

struct AST
{
};

struct TokenNode
{
    struct TokenNode   *next;
    ETokenType          type;
    union
    {
        EFunctionType       f_type;
        EType               t_type;
        EOperator           o_type;
        char               *value;
    }                   data;
};

TokenNode *token_node_create()
{
    TokenNode *node     =   NULL;
    node                =   (TokenNode *)malloc(sizeof(*node));
    if (node == NULL)
    {
        goto exit;
    }
    node->data.value    =   NULL;
    node->next          =   NULL;
    node->type          =   T_UNKNOWN;

exit:
    return node;
}

Token *token_create()
{
    Token *token    =   NULL;
    token           =   (Token *)malloc(sizeof(*token));
    if (token == NULL)
    {
        goto exit;
    }

    TokenNode *node =   token_node_create();
    if (node == NULL)
    {
        free(token);
        token = NULL;
        goto exit;
    }

    token->head = node;
    token->last = node;
    token->size = 0;

exit:
    return token;
}

void token_free(Token **token)
{
    Token       *list;
    TokenNode   *current, *next;

    if (token == NULL || *token == NULL) goto exit;

    list    =   *token;
    current =   list->head->next;

    while (current != NULL)
    {
        next    =   current->next;

        if (current->type == T_UNKNOWN && current->data.value != NULL)
            free(current->data.value);

        free(current);
        current =   next;
    }

    free(list->head);
    free(list);

exit:
    return;
}

void token_append_node(Token *token, TokenNode *node)
{
    if (node == NULL) return;
    TokenNode *tmp      =   token->last;
    tmp->next           =   node;
    token->last         =   node;

    ++token->size;
}

EFunctionType is_function_declaration(char *buf)
{
    if (strcmp(buf, "รับค่า") == 0)
        return F_PARAM;
    else if (strcmp(buf, "ไม่รับค่า") == 0)
        return F_NO_PARAM;

    return F_UNKNOWN;
}

EType is_type(char *buf)
{
    if (strcmp(buf, "จำนวนเต็ม") == 0)
        return TT_INT;

    return TT_UNKNOWN;
}

EOperator is_operator(char *buf)
{
    if (strcmp(buf, "คืนค่า") == 0)
        return O_RETURN;
    return O_UNKNOWN;
}

TokenNode *tokenize_buf(char *buf, size_t len)
{
    char      *cpy  =   malloc(sizeof(char) * len + 1);
    TokenNode *node =   token_node_create();
    size_t     i    =   0;

    EFunctionType   f_type;
    EType           t_type;
    EOperator       o_type;

    for (i = 0; i < len; ++i)
        cpy[i] = buf[i];

    cpy[len] = '\0';

    if ((f_type = is_function_declaration(cpy)) != F_UNKNOWN)
    {
        node->type = T_FUNC;
        node->data.f_type = f_type;
    }
    else if ((t_type = is_type(cpy)) != TT_UNKNOWN)
    {
        node->type = T_TYPE;
        node->data.t_type = t_type;
    }
    else if ((o_type = is_operator(cpy)) != O_UNKNOWN)
    {
        node->type = T_OPT;
        node->data.o_type = o_type;
    }
    else
    {
        node->type = T_UNKNOWN;
        node->data.value = cpy;
    }

    if (node->type != T_UNKNOWN)
    {
        free(cpy);
    }

    return node;
}

Token *tokenizer(FILE *file)
{
    Token  *token       =   token_create();
    char    c           =   0;
    size_t  count       =   0;
    char   *buf         =   NULL;
    size_t  buf_size    =   sizeof(char) * PARSER_BUFFER_SIZE;

    buf     =   malloc(buf_size);
    
    while ((c = fgetc(file)) != EOF)
    {
        if (count >= buf_size) break;

        if (c == ' ' || c == '\n')
        {
            if (count == 0) continue;
            
            buf[count] = '\0';
            TokenNode *node = tokenize_buf(buf, count);
            token_append_node(token, node);
            count = 0;
        }
        else
        {
            buf[count] = c;
            ++count;
        }
    }

    if (count > 0)
    {
        buf[count] = '\0';
        TokenNode *node = tokenize_buf(buf, count);
        token_append_node(token, node);
    }

    free(buf);
    return token;
}

void print_token(Token *token)
{
    if (token == NULL || token->head == NULL) return;

    TokenNode *current = token->head->next;
    while (current != NULL)
    {
        switch (current->type)
        {
            case T_TYPE:
                printf("TYPE");
                break;
            case T_FUNC:
                printf("FUNC");
                break;
            case T_OPT:
                printf("OPT");
                break;
            case T_UNKNOWN:
                printf("%s", current->data.value);
                break;
        }

        putchar(' ');
        current = current->next;
    }
}

AST *parser(Token *token)
{
    return NULL;
}

void ast_free(AST **ast)
{
    if (ast == NULL || *ast == NULL) return;

    free(ast);
}

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

    fclose(fptr);
    ast_free(&ast);
    token_free(&token);

    return 0;
error:
    return 1;
}
