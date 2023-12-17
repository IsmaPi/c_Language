#include "include/io.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char* filename = argv[1];
    char* file_contents = get_file_contents(filename);

    if (!file_contents) {
        fprintf(stderr, "Error reading file: %s\n", filename);
        return EXIT_FAILURE;
    }

    lexer_T* lexer = init_lexer(file_contents);
    parser_T* parser = init_parser(lexer);
    AST_T* root = parser_parse(parser, NULL);  // Assuming the second argument is a scope, NULL for global scope

    if (root == NULL) {
        // Handle parsing error
        fprintf(stderr, "Error: Parsing failed.\n");
        free(file_contents);
        // Free lexer and parser if necessary
        return EXIT_FAILURE;
    }

    visitor_T* visitor = init_visitor();
    visitor_visit(visitor, root);  // Execute/interpret the AST

    // Clean up
    free(file_contents);
    // Add code here to free lexer, parser, visitor, and the AST nodes

    return EXIT_SUCCESS;
}