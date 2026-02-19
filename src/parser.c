#include "parser.h"
#include <stdlib.h>

Stack *stack_create()
{
    Stack *stack = NULL;
    size_t alloc = sizeof(*stack) + sizeof(EGrammarType) * STACK_INIT_SIZE;
    stack = malloc(alloc);

    stack->size = STACK_INIT_SIZE;
    stack->length = 0;

    return stack;
}

void stack_push(Stack *stack, EGrammarType type)
{
    if (stack->size == stack->length + 1)
    {
        stack_expand(stack, stack->size * 2);
    }
    stack->stack[stack->length] = type;
    ++stack->length;
}

EGrammarType stack_top(Stack *stack)
{
    if (stack->length == 0)
        return -1;
    return stack->stack[stack->length - 1];
}

void stack_pop(Stack *stack)
{
    --stack->length;
}

void stack_expand(Stack *stack, size_t size)
{
    stack->size = size;
    realloc(stack->stack, sizeof(Stack) + size);
}

void stack_free(Stack **stack)
{
    if (stack == NULL || *stack == NULL)
        return;

    free(*stack);
    stack = NULL;
}

ASTNode *ast_node_create()
{
    ASTNode *node = NULL;
    node = malloc(sizeof(*node));
    node->next = NULL;
    node->type = G_UNKNOWN;
    node->value = NULL;
    return node;
}

void ast_node_free(ASTNode **node)
{
    if (node == NULL || *node == NULL)
        return;

    free(*node);
    node = NULL;
}

AST *ast_create()
{
    AST *ast = NULL;
    ast = malloc(sizeof(*ast));

    ASTNode *dummy = NULL;
    dummy = ast_node_create();
    dummy->type = G_EXPR;

    ast->head = dummy;
    ast->last = dummy;
    ast->size = 0;

    return ast;
}

void ast_free(AST **ast)
{
    if (ast == NULL || *ast == NULL)
        return;

    ASTNode *cur = (*ast)->head;

    while (cur != NULL)
    {
        ASTNode *tmp = cur->next;
        ast_node_free(&cur);
        cur = tmp;
    }

    free(*ast);
    ast = NULL;
}

void expression(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_FUNC:
        break;
    case T_TYPE:
        stack_push(stack, G_EXPR);
        stack_push(stack, G_VALUE);
        stack_push(stack, G_IS);
        stack_push(stack, G_IDENTIFIER);
        stack_push(stack, G_TYPE);
        break;
    case T_OPT:
        break;
    case T_UNKNOWN:
        stack_push(stack, G_EXPR);
        stack_push(stack, G_TYPE_OR_IDEN);
        //        stack_push(stack, G_EXPR);
        //        stack_push(stack, G_TYPE);
        //        stack_push(stack, G_RET_TYPE);
        //        stack_push(stack, G_PARAM_TYPE);
        //        stack_push(stack, G_IDENTIFIER);
        break;
    }
}

void identifier(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_UNKNOWN:
        // register name
    default:
        // TODO:handle wrong param
        break;
    }
}

void function_param_type(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_UNKNOWN:
        // register name
    default:
        // TODO:handle wrong param
        break;
    }
}

void operation(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_UNKNOWN:
        // register name
    default:
        // TODO:handle wrong param
        break;
    }
}

void function_return_type(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_UNKNOWN:
        // register name
    default:
        // TODO:handle wrong param
        break;
    }
}

void type(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_UNKNOWN:
        // register name
    default:
        // TODO:handle wrong param
        break;
    }
}

void is(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_UNKNOWN:
        // register name
    default:
        // TODO:handle wrong param
        break;
    }
}

void value(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_UNKNOWN:
        // register name
    default:
        // TODO:handle wrong param
        break;
    }
}

void type_or_identifier(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_UNKNOWN:
        // register name
    default:
        // TODO:handle wrong param
        break;
    }
}

void expression_or_value(Stack *stack, TokenNode *token)
{
    stack_pop(stack);
    if (token == NULL)
        return;
    switch (token->type)
    {
    case T_UNKNOWN:
        // register name
    default:
        // TODO:handle wrong param
        break;
    }
}

void print_stack(Stack *stack)
{
    putc('[', stdout);
    for (int i = 0; i < stack->length; ++i)
    {
        switch (stack->stack[i])
        {
        case G_EXPR:
            printf("EXPR");
            break;
        case G_IDENTIFIER:
            printf("IDENTIFIER");
            break;
        case G_PARAM_TYPE:
            printf("PARAM_TYPE");
            break;
        case G_RET_TYPE:
            printf("RET_TYPE");
            break;
        case G_TYPE:
            printf("TYPE");
            break;
        case G_IS:
            printf("IS");
            break;
        case G_VALUE:
            printf("VALUE");
            break;
        case G_TYPE_OR_IDEN:
            printf("TYPE_OR_IDENTIFIER");
            break;
        case G_EXPR_OR_VALUE:
            printf("EXPR_OR_VALUE");
            break;
        case G_UNKNOWN:
            printf("UNKNOWN");
            break;
        }

        if (i != stack->length - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

AST *parser(Token *token)
{
    printf("Tokens: \n");
    print_token(token);

    AST *ast = ast_create();
    Stack *stack = stack_create();
    stack_push(stack, G_EXPR);

    TokenNode *cur = token->head;
    if (cur->next != NULL)
        cur = cur->next;

    int line = 1;
    while (stack->length > 0)
    {
        printf("%d: ", line);
        if (cur != NULL)
        {
            print_token_node(cur);
            putchar('\n');
        }
        print_stack(stack);
        ++line;
        switch (stack_top(stack))
        {
        case G_EXPR:
            expression(stack, cur);
            if (stack_top(stack) == G_TYPE_OR_IDEN && cur != NULL)
                cur = cur->next;
            break;
        case G_IDENTIFIER:
            identifier(stack, cur);
            if (cur != NULL)
                cur = cur->next;
            break;
        case G_PARAM_TYPE:
            function_param_type(stack, cur);
            if (cur != NULL)
                cur = cur->next;
            break;
        case G_RET_TYPE:
            function_return_type(stack, cur);
            if (cur != NULL)
                cur = cur->next;
            break;
        case G_TYPE:
            type(stack, cur);
            if (cur != NULL)
                cur = cur->next;
            break;
        case G_IS:
            is(stack, cur);
            if (cur != NULL)
                cur = cur->next;
            break;
        case G_VALUE:
            value(stack, cur);
            if (cur != NULL)
                cur = cur->next;
            break;
        case G_TYPE_OR_IDEN:
            type_or_identifier(stack, cur);
            break;
        case G_EXPR_OR_VALUE:
            expression_or_value(stack, cur);
            break;
        case G_UNKNOWN:
            break;
        }
    }

    stack_free(&stack);

    return ast;
}
