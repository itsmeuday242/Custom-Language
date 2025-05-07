#include "include/token.h"

token_T *init_token(TOKEN_STRUCT::TOKEN type, string value)
{
    token_T *token = new token_T();
    token->type = type;
    token->value = value;

    return token;
}
