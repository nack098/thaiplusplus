#include "token.h"

#include <stdlib.h>
#include <string.h>

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
    else if (strcmp(buf, "จำนวนทศนิยม") == 0)
        return TT_FLOAT;

    return TT_UNKNOWN;
}

EOperator is_operator(char *buf)
{
    if (strcmp(buf, "คืนค่า") == 0)
        return O_RETURN;
    else if (strcmp(buf, "ประเภท") == 0)
        return O_CLASS_DECLARATION;
    else if (strcmp(buf, "ใช้ภายใน") == 0)
        return O_PRIVATE;
    else if (strcmp(buf, "ใช้ภายนอก") == 0)
        return O_PUBLIC;
    else if (strcmp(buf, "ขยาย") == 0)
        return O_INHERIT;
    else if (strcmp(buf, "จาก") == 0)
        return O_FROM;
    else if (strcmp(buf, "เรียก") == 0)
        return O_CALL;
    else if (strcmp(buf, "เรียกใช้") == 0)
        return O_INCLUDE;
    else if (strcmp(buf, "จากภาษา") == 0)
        return O_EXT_LANG;
    else if (strcmp(buf, "เป็น") == 0)
        return O_IS;
    else if (strcmp(buf, "ไม่") == 0)
        return O_NOT;
    else if (strcmp(buf, "เท่ากับ") == 0)
        return O_EQUAL;
    else if (strcmp(buf, "มากกว่า") == 0)
        return O_GREATER;
    else if (strcmp(buf, "มากกว่าหรือเท่ากับ") == 0)
        return O_GREATER_OR_EQUAL;
    else if (strcmp(buf, "น้อยกว่า") == 0)
        return O_LESS;
    else if (strcmp(buf, "น้อยกว่าหรือเท่ากับ") == 0)
        return O_LESS_OR_EQUAL;
    else if (strcmp(buf, "ถ้า") == 0)
        return O_IF;
    else if (strcmp(buf, "แล้ว") == 0)
        return O_ELSE;

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
                switch (current->data.t_type)
                {
                    case TT_INT:
                        printf("(INT)");
                        break;
                    case TT_FLOAT:
                        printf("(FLOAT)");
                        break;
                    case TT_UNKNOWN:
                        printf("(UKNOWN)");
                        break;
                }
                break;
            case T_FUNC:
                printf("FUNC");
                switch (current->data.f_type)
                {
                    case F_PARAM:
                        printf("(PARAM)");
                        break;
                    case F_NO_PARAM:
                        printf("(NOPARAM)");
                        break;
                    case F_UNKNOWN:
                        printf("(UNKNOWN)");
                        break;
                }
                break;
            case T_OPT:
                printf("OPT");
                switch (current->data.o_type)
                {
                    case O_RETURN:
                        printf("(RETURN)");
                        break;
                    case O_CLASS_DECLARATION:
                        printf("(CLASS)");
                        break;
                    case O_PRIVATE:
                        printf("(PRIVATE)");
                        break;
                    case O_PUBLIC:
                        printf("(PUBLIC)");
                        break;
                    case O_INHERIT:
                        printf("(INHERIT)");
                        break;
                    case O_FROM:
                        printf("(FROM)");
                        break;
                    case O_CALL:
                        printf("(CALL)");
                        break;
                    case O_INCLUDE:
                        printf("(INCLUDE)");
                        break;
                    case O_IS:
                        printf("(INCLUDE)");
                        break;
                    case O_EQUAL:
                        printf("(EQUAL)");
                        break;
                    case O_GREATER:
                        printf("(GREATER)");
                        break;
                    case O_GREATER_OR_EQUAL:
                        printf("(GREATER_OR_EQUAL)");
                        break;
                    case O_LESS:
                        printf("(LESS)");
                        break;
                    case O_LESS_OR_EQUAL:
                        printf("(LESS_OR_EQUAL)");
                        break;
                    case O_NOT:
                        printf("(NOT)");
                        break;
                    case O_EXT_LANG:
                        printf("(EXT_LANG)");
                        break;
                    case O_IF:
                        printf("(IF)");
                        break;
                    case O_ELSE:
                        printf("(ELSE)");
                        break;
                    case O_UNKNOWN:
                        printf("(UKNOWN)");
                        break;
                }
                break;
            case T_UNKNOWN:
                printf("UNKNOWN");
                break;
        }

        putchar(' ');
        current = current->next;
    }
}
