#ifndef TOKEN_H
#define TOKEN_H

#include <string>
using namespace std;

typedef struct TOKEN_STRUCT
{
    enum TOKEN
    {
        TOKEN_ID,
        TOKEN_EQUALS,
        TOKEN_STRING,
        TOKEN_SEMI,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_RBRACE,
        TOKEN_LBRACE,
        TOKEN_COMMA,
        TOKEN_EOF
    } type;

    string value;
} token_T;

token_T *init_token(TOKEN_STRUCT::TOKEN type, string value);
#endif
