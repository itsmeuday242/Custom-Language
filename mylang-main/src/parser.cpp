#include "include/parser.h"
#include "include/scope.h"

#include <iostream>
using namespace std;

parser_T *init_parser(lexer_T *lexer)
{
    parser_T *parser = new parser_T();
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;

    parser->scope = init_scope();

    return parser;
}

void parser_eat(parser_T *parser, TOKEN_STRUCT::TOKEN token_type)
{
    if (parser->current_token->type == token_type)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        cerr << "Unexpected token `"
             << parser->current_token->value
             << "`, with type "
             << parser->current_token->type
             << endl;
        exit(1);
    }
}

AST_T *parser_parse(parser_T *parser, scope_T *scope)
{
    return parser_parse_statements(parser, scope);
}

AST_T *parser_parse_statement(parser_T *parser, scope_T *scope)
{
    switch (parser->current_token->type)
    {
    case TOKEN_STRUCT::TOKEN_ID:
        return parser_parse_id(parser, scope);
    }

    return init_ast(AST_STRUCT::AST_NOOP);
}

AST_T *parser_parse_statements(parser_T *parser, scope_T *scope)
{
    // every statement ending with a semicolon
    AST_T *compound = init_ast(AST_STRUCT::AST_COMPOUND);
    compound->scope = scope;
    compound->compound_value = vector<AST_T *>();

    AST_T *ast_statement = parser_parse_statement(parser, scope);
    ast_statement->scope = scope;
    compound->compound_value.push_back(ast_statement);

    while (parser->current_token->type == TOKEN_STRUCT::TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_STRUCT::TOKEN_SEMI);

        AST_T *ast_statement = parser_parse_statement(parser, scope);

        if (ast_statement)
            compound->compound_value.push_back(ast_statement);
    }

    return compound;
}

AST_T *parser_parse_expr(parser_T *parser, scope_T *scope)
{
    switch (parser->current_token->type)
    {
    case TOKEN_STRUCT::TOKEN_STRING:
        return parser_parse_string(parser, scope);
    case TOKEN_STRUCT::TOKEN_ID:
        return parser_parse_id(parser, scope);
    }

    return init_ast(AST_STRUCT::AST_NOOP);
}

AST_T *parser_parse_factor(parser_T *parser, scope_T *scope)
{
    cerr << "under construction" << endl;
    exit(1);
}

AST_T *parser_parse_term(parser_T *parser, scope_T *scope)
{
    cerr << "under construction" << endl;
    exit(1);
}

AST_T *parser_parse_function_call(parser_T *parser, scope_T *scope)
{
    AST_T *function_call = init_ast(AST_STRUCT::AST_FUNCTION_CALL);

    function_call->function_call_name = parser->prev_token->value;
    parser_eat(parser, TOKEN_STRUCT::TOKEN_LPAREN);

    function_call->function_call_arguments = vector<AST_T *>();

    AST_T *ast_expr = parser_parse_expr(parser, scope);
    function_call->function_call_arguments.push_back(ast_expr);

    while (parser->current_token->type == TOKEN_STRUCT::TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_STRUCT::TOKEN_COMMA);

        AST_T *ast_expr = parser_parse_expr(parser, scope);
        function_call->function_call_arguments.push_back(ast_expr);
    }
    parser_eat(parser, TOKEN_STRUCT::TOKEN_RPAREN);

    function_call->scope = scope;

    return function_call;
}

AST_T *parser_parse_variable_definition(parser_T *parser, scope_T *scope)
{
    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID); // var
    string variable_definition_variable_name = parser->current_token->value;

    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID); // var name
    parser_eat(parser, TOKEN_STRUCT::TOKEN_EQUALS);
    AST_T *variable_definition_value = parser_parse_expr(parser, scope);

    AST_T *variable_definition = init_ast(AST_STRUCT::AST_VARIABLE_DEFINITION);
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_definition_value = variable_definition_value;

    variable_definition->scope = scope;

    return variable_definition;
}

AST_T *parser_parse_function_definition(parser_T *parser, scope_T *scope)
{
    AST_T *ast = init_ast(AST_STRUCT::AST_FUNCTION_DEFINITION);
    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID); // function

    string function_name = parser->current_token->value;
    ast->function_definition_name = function_name;

    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID); // function name

    parser_parse_function_args(ast, parser, scope);

    parser_eat(parser, TOKEN_STRUCT::TOKEN_LBRACE);

    ast->function_definition_body = parser_parse_statements(parser, scope);

    parser_eat(parser, TOKEN_STRUCT::TOKEN_RBRACE);

    ast->scope = scope;

    return ast;
}
void parser_parse_function_args(AST_T *ast, parser_T *parser, scope_T *scope)
{
    parser_eat(parser, TOKEN_STRUCT::TOKEN_LPAREN);

    ast->function_definition_args = vector<AST_T *>();

    // If there are no arguments in the function
    if (parser->current_token->type == TOKEN_STRUCT::TOKEN_RPAREN)
    {
        parser_eat(parser, TOKEN_STRUCT::TOKEN_RPAREN);
        return;
    }
    AST_T *arg = parser_parse_variable(parser, scope);
    ast->function_definition_args.push_back(arg);

    while (parser->current_token->type == TOKEN_STRUCT::TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_STRUCT::TOKEN_COMMA);
        AST_T *arg = parser_parse_variable(parser, scope);
        ast->function_definition_args.push_back(arg);
    }

    parser_eat(parser, TOKEN_STRUCT::TOKEN_RPAREN);
}

AST_T *parser_parse_variable(parser_T *parser, scope_T *scope)
{
    string token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_STRUCT::TOKEN_ID); // var name or function call name

    if (parser->current_token->type == TOKEN_STRUCT::TOKEN_LPAREN)
        return parser_parse_function_call(parser, scope);

    AST_T *ast_variable = init_ast(AST_STRUCT::AST_VARIABLE);
    ast_variable->variable_name = token_value;

    ast_variable->scope = scope;

    return ast_variable;
}

AST_T *parser_parse_string(parser_T *parser, scope_T *scope)
{
    AST_T *ast_string = init_ast(AST_STRUCT::AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRUCT::TOKEN_STRING);

    ast_string->scope = scope;

    return ast_string;
}

AST_T *parser_parse_id(parser_T *parser, scope_T *scope)
{

    if ("var" == parser->current_token->value)
    {
        return parser_parse_variable_definition(parser, scope);
    }
    else if ("function" == parser->current_token->value)
    {
        return parser_parse_function_definition(parser, scope);
    }
    else
    {
        return parser_parse_variable(parser, scope);
    }
}
