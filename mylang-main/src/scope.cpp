#include "include/scope.h"

#include <iostream>
using namespace std;


scope_T *init_scope()
{
    scope_T *scope = new scope_T();
    return scope;
}

AST_T *scope_add_function_definition(scope_T *scope, AST_T *fdef)
{
    if(scope->function_definitions.count(fdef->function_definition_name))
    {
        cerr<<"Function `"<<fdef->function_definition_name<<"` already defined"<<endl;
        exit(1);
    }
    scope->function_definitions[fdef->function_definition_name] = fdef;
    return fdef;
}

AST_T *scope_get_function_definition(scope_T *scope, const string fname)
{
    auto it = scope->function_definitions.find(fname);
    if(it != scope->function_definitions.end())
    {
        return it->second;
    }

    return nullptr;
}

AST_T *scope_add_variable_definition(scope_T *scope, AST_T *vdef)
{
    if(scope->variable_definitions.count(vdef->variable_definition_variable_name))
    {
        cerr<<"Variable `"<<vdef->variable_definition_variable_name<<"` already defined"<<endl;
        exit(1);
    }
    scope->variable_definitions[vdef->variable_definition_variable_name] = vdef;
    return vdef;
}

AST_T *scope_get_variable_definition(scope_T *scope, const string name)
{
    auto it = scope->variable_definitions.find(name);
    if(it != scope->variable_definitions.end())
    {
        return it->second;
    }
    return nullptr;
}
