#include "include/visitor.h"
#include "include/scope.h"

#include <iostream>
using namespace std;

static AST_T *builtin_function_print(visitor_T *visitor, vector<AST_T *> args)
{
    for (int i = 0; i < args.size(); i++)
    {
        AST_T *visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
        case AST_STRUCT::AST_STRING:
            cout << visited_ast->string_value << endl;
            break;
        default:
            cerr << "Type not supported : `" << visited_ast->type << "`, memory : " << visited_ast << endl;
            break;
        }
    }

    return init_ast(AST_STRUCT::AST_NOOP);
}

static AST_T *builtin_function_exit(visitor_T *visitor, vector<AST_T *> args)
{
    for (int i = 0; i < args.size(); i++)
    {
        AST_T *visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
        case AST_STRUCT::AST_NOOP:
            cout << "You exited" << endl;
            exit(0);
            break;
        default:
            cerr << "Type not supported : [" << visited_ast->type << "], memory : " << visited_ast << endl;
            break;
        }
    }

    return init_ast(AST_STRUCT::AST_NOOP);
}

static AST_T *builtin_function_clear(visitor_T *visitor, vector<AST_T *> args)
{
    for (int i = 0; i < args.size(); i++)
    {
        AST_T *visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
        case AST_STRUCT::AST_NOOP:
            system("clear");
            break;
        default:
            cerr << "Type not supported : [" << visited_ast->type << "], memory : " << visited_ast << endl;
            break;
        }
    }

    return init_ast(AST_STRUCT::AST_NOOP);
}

visitor_T *init_visitor()
{
    visitor_T *visitor = new visitor_T();

    return visitor;
}

AST_T *visitor_visit(visitor_T *visitor, AST_T *node)
{
    switch (node->type)
    {
    case AST_STRUCT::AST_VARIABLE_DEFINITION:
        return visitor_visit_variable_definition(visitor, node);
        break;
    case AST_STRUCT::AST_FUNCTION_DEFINITION:
        return visitor_visit_function_definition(visitor, node);
        break;
    case AST_STRUCT::AST_VARIABLE:
        return visitor_visit_variable(visitor, node);
        break;
    case AST_STRUCT::AST_FUNCTION_CALL:
        return visitor_visit_function_call(visitor, node);
        break;
    case AST_STRUCT::AST_STRING:
        return visitor_visit_string(visitor, node);
        break;
    case AST_STRUCT::AST_COMPOUND:
        return visitor_visit_compound(visitor, node);
        break;
    case AST_STRUCT::AST_NOOP:
        return node;
        break;
    }

    cerr << "Uncaught statement of type " << node->type << endl;
    exit(1);

    return init_ast(AST_STRUCT::AST_NOOP);
}

AST_T *visitor_visit_variable_definition(visitor_T *visitor, AST_T *node)
{
    scope_add_variable_definition(
        node->scope,
        node);

    return node;
}

AST_T *visitor_visit_function_definition(visitor_T *visitor, AST_T *node)
{
    scope_add_function_definition(node->scope, node);

    return node;
}

AST_T *visitor_visit_variable(visitor_T *visitor, AST_T *node)
{
    AST_T *vdef = scope_get_variable_definition(
        node->scope,
        node->variable_name);

    if (vdef)
        return visitor_visit(visitor, vdef->variable_definition_value);

    cerr << "Undefined variable `" << node->variable_name << "`\n";
    exit(1);
}

AST_T *visitor_visit_function_call(visitor_T *visitor, AST_T *node)
{
    if ("print" == node->function_call_name)
    {
        return builtin_function_print(visitor, node->function_call_arguments);
    }
    if ("exit" == node->function_call_name)
    {
        return builtin_function_exit(visitor, node->function_call_arguments);
    }

    if ("clear" == node->function_call_name)
    {
        return builtin_function_clear(visitor, node->function_call_arguments);
    }

    // user defined functions

    AST_T *fdef = scope_get_function_definition(node->scope, node->function_call_name);

    if (!fdef)
    {
        cerr << "Undefined method `" << node->function_call_name << "`\n";
        exit(1);
    }

    for (int i = 0; i < node->function_call_arguments.size(); i++)
    {
        if (node->function_call_arguments[i]->type == AST_STRUCT::AST_NOOP)
            break;

        if (i >= fdef->function_definition_args.size())
        {
            cerr << "Too many arguments provided to function `" << node->function_call_name << "`" << endl;
            exit(1);
        }
        // grab the variable from the function definition arguments
        AST_T *ast_var = (AST_T *)fdef->function_definition_args[i];

        // grab the value from the function call arguments
        AST_T *ast_value = (AST_T *)node->function_call_arguments[i];

        // create a new variable definition with the value of the argument
        // in the function call.
        AST_T *ast_vardef = init_ast(AST_STRUCT::AST_VARIABLE_DEFINITION);
        ast_vardef->variable_definition_value = ast_value;

        // copy the name from the function definition argument into the new
        // variable definition
        ast_vardef->variable_definition_variable_name = ast_var->variable_name;

        // push our variable definition into the function body scope.
        scope_add_variable_definition(fdef->function_definition_body->scope, ast_vardef);
    }

    return visitor_visit(visitor, fdef->function_definition_body);
}

AST_T *visitor_visit_string(visitor_T *visitor, AST_T *node)
{
    return node;
}

AST_T *visitor_visit_compound(visitor_T *visitor, AST_T *node)
{

    for (int i = 0; i < node->compound_value.size(); i++)
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_STRUCT::AST_NOOP);
}
