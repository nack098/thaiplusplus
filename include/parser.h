#pragma once
#include "./token.h"
#define STACK_INIT_SIZE 256

typedef enum
{
    G_EXPR,
    G_TYPE_OR_IDEN,
    G_EXPR_OR_VALUE,
    G_IDENTIFIER,
    G_PARAM_TYPE,
    G_RET_TYPE,
    G_TYPE,
    G_IS,
    G_VALUE,
    G_UNKNOWN,
} EGrammarType;

typedef struct ASTNode ASTNode;
typedef struct AST AST;
typedef struct Stack Stack;

Stack *stack_create();
void stack_push(Stack *, EGrammarType);
void stack_expand(Stack *, size_t);
EGrammarType stack_top(Stack *);
void stack_pop(Stack *);
void stack_free(Stack **);

ASTNode *ast_node_create();
void ast_node_free(ASTNode **);

AST *ast_create();
void ast_append(AST *, EGrammarType);
void ast_free(AST **);

AST *parser(Token *);

struct AST
{
    ASTNode *head;
    ASTNode *last;
    size_t size;
};

struct ASTNode
{
    struct ASTNode *next;
    EGrammarType type;
    void *value;
};

struct Stack
{
    size_t size;
    size_t length;
    EGrammarType stack[];
};
