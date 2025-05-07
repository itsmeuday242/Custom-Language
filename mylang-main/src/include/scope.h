#ifndef SCOPE_H
#define SCOPE_H
#include "AST.h"

#include <map>
#include <string>
using namespace std;

typedef struct SCOPE_STRUCT
{
    map<string, AST_T *> function_definitions;

    map<string, AST_T *> variable_definitions;
} scope_T;

scope_T *init_scope();

AST_T *scope_add_function_definition(scope_T *scope, AST_T *fdef);

AST_T *scope_get_function_definition(scope_T *scope, const string fname);

AST_T *scope_add_variable_definition(scope_T *scope, AST_T *vdef);

AST_T *scope_get_variable_definition(scope_T *scope, const string name);
#endif
