#include "include/AST.h"


AST_T* init_ast(AST_STRUCT::AST_NODE_TYPE type)
{
    AST_T* ast = new AST_T();
    ast->type = type;
    return ast;
}
