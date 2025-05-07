#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
using namespace std;

typedef struct AST_STRUCT
{
    enum AST_NODE_TYPE
    {
        AST_VARIABLE_DEFINITION,
        AST_FUNCTION_DEFINITION,
        AST_VARIABLE,
        AST_FUNCTION_CALL,
        AST_STRING,
        AST_COMPOUND,
        AST_NOOP
    } type;

    struct SCOPE_STRUCT* scope;

    /* AST_VARIABLE_DEFINITION */
    string variable_definition_variable_name;
    AST_STRUCT *variable_definition_value;

    /* AST_FUNCTION_DEFINITION */
    AST_STRUCT *function_definition_body;
    string function_definition_name;
    vector<AST_STRUCT *> function_definition_args;

    /* AST_VARIABLE */
    string variable_name;

    /* AST_FUNCTION_CALL */
    string function_call_name;
    vector<AST_STRUCT *> function_call_arguments;

    /* AST_STRING */
    string string_value;

    /* AST_COMPOUND */
    vector<AST_STRUCT *> compound_value;
} AST_T;

AST_T *init_ast(AST_STRUCT::AST_NODE_TYPE type);
#endif
