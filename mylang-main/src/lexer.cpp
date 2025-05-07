#include "include/lexer.h"

lexer_T *init_lexer(string contents)
{
    lexer_T *lexer = new lexer_T();
    lexer->contents = contents;
    lexer->i = 0;
    lexer->c = contents[lexer->i];

    return lexer;
}

void lexer_advance(lexer_T *lexer)
{
    if (lexer->c != '\0' and lexer->i < lexer->contents.size())
    {
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
}

void lexer_skip_whitespace(lexer_T *lexer)
{
    while (lexer->c == ' ' or lexer->c == '\n' or lexer->c == '\r')
    {
        lexer_advance(lexer);
    }
}

token_T *lexer_get_next_token(lexer_T *lexer)
{

    while (lexer->c != '\0' and lexer->i < lexer->contents.size())
    {
        if (lexer->c == ' ' or lexer->c == '\n' or lexer->c == '\r')
            lexer_skip_whitespace(lexer);

        if (isalnum(lexer->c))
            return lexer_collect_id(lexer);

        if (lexer->c == '"')
            return lexer_collect_string(lexer);

        switch (lexer->c)
        {
        case '=':
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_EQUALS, lexer_get_current_char_as_string(lexer)));
            break;
        case ';':
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_SEMI, lexer_get_current_char_as_string(lexer)));
            break;
        case '(':
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_LPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        case ')':
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_RPAREN, lexer_get_current_char_as_string(lexer)));
            break;
        case '{':
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_LBRACE, lexer_get_current_char_as_string(lexer)));
            break;
        case '}':
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_RBRACE, lexer_get_current_char_as_string(lexer)));
            break;
        case ',':
            return lexer_advance_with_token(lexer, init_token(TOKEN_STRUCT::TOKEN_COMMA, lexer_get_current_char_as_string(lexer)));
            break;
        }
    }

    return init_token(TOKEN_STRUCT::TOKEN_EOF, "\0");
}

token_T *lexer_collect_string(lexer_T *lexer)
{
    lexer_advance(lexer);

    string value;

    while (lexer->c != '"')
    {
        string s = lexer_get_current_char_as_string(lexer);
        value += s;

        lexer_advance(lexer);
    }

    lexer_advance(lexer);

    return init_token(TOKEN_STRUCT::TOKEN_STRING, value);
}

token_T *lexer_collect_id(lexer_T *lexer)
{
    string value;

    while (isalnum(lexer->c) or lexer->c == '_')
    {
        string s = lexer_get_current_char_as_string(lexer);
        value += s;

        lexer_advance(lexer);
    }

    return init_token(TOKEN_STRUCT::TOKEN_ID, value);
}

token_T *lexer_advance_with_token(lexer_T *lexer, token_T *token)
{
    lexer_advance(lexer);

    return token;
}

string lexer_get_current_char_as_string(lexer_T *lexer)
{
    return string(1, lexer->c);
}
