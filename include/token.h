#pragma once

#include <stdio.h>
#define PARSER_BUFFER_SIZE 256

typedef struct TokenNode TokenNode;
TokenNode  *token_node_create();

typedef struct Token Token;
Token      *token_create();
void        token_append_node(Token*, TokenNode*);
void        token_free(Token**);

Token      *tokenizer(FILE*);
void        print_token(Token*);

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
    TT_FLOAT,
    TT_UNKNOWN,
} EType;

typedef enum
{
    O_CLASS_DECLARATION,
    O_RETURN,
    O_PRIVATE,
    O_PUBLIC,
    O_FROM,
    O_INHERIT,
    O_CALL,
    O_INCLUDE,
    O_EXT_LANG,
    O_IS,
    O_NOT,
    O_EQUAL,
    O_LESS,
    O_LESS_OR_EQUAL,
    O_GREATER,
    O_GREATER_OR_EQUAL,
    O_IF,
    O_ELSE,
    O_UNKNOWN,
} EOperator;

struct Token
{
    TokenNode  *head;
    TokenNode  *last;
    size_t      size;
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
