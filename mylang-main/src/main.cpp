#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{

    if (argc >= 2)
    {
        string file_contents = get_file_contents(argv[1]);
        lexer_T *lexer = init_lexer(file_contents);
        parser_T *parser = init_parser(lexer);
        AST_T *root = parser_parse(parser, parser->scope);
        visitor_T *visitor = init_visitor();
        visitor_visit(visitor, root);
    }
    else
    {
        cout << "No input file provided" << endl;
        cout << "Usage:" << endl
             << "[sudo] make clean install" << endl
             << "./mylang.out <input_file>" << endl;
    }
    return 0;
}