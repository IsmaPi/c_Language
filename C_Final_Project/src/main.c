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
    AST_T* root = parser_parse(parser, NULL);

    if (root == NULL) {
        fprintf(stderr, "Error: Parsing failed.\n");
        free(file_contents);
        return EXIT_FAILURE;
    }

    visitor_T* visitor = init_visitor();
    AST_T* result = visitor_visit(visitor, root);

    // Check if result is NULL to handle any runtime errors
    if (result == NULL) {
        fprintf(stderr, "Runtime error during execution.\n");
        free(file_contents);
        // Free other resources here
        return EXIT_FAILURE;
    }

    // Clean up resources
    free(file_contents);

    return EXIT_SUCCESS;
}