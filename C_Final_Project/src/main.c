#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
#include "include/io.h"

// Adjust the maximum input limit as needed
#define MAX_INPUT_LIMIT 1024

void print_help() {
    // Update usage message for your language
    printf("Usage:\nmathlang.out <filename.math>\n");
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        // Loop through the arguments to handle multiple files
        for (int i = 1; i < argc; i++) {
            int len = strlen(argv[i]);
            // Check that the file has the correct `.math` extension
            char* file_extension = &argv[i][len - 5];
            if (strcmp(file_extension, ".math") == 0) {
                // Initialize lexer, parser, and visitor with the file contents
                lexer_T* lexer = init_lexer(get_file_contents(argv[i]));
                parser_T* parser = init_parser(lexer);
                AST_T* root = parser_parse(parser, parser->scope);
                visitor_T* visitor = init_visitor();
                visitor_visit(visitor, root);
            } else {
                print_help();
            }
        }
    } else {
        // Interactive mode for the language
        char input[MAX_INPUT_LIMIT];
        printf("Welcome to MathLang! Type your expressions below.\n");
        while (1) {
            printf("MathLang> ");
            fgets(input, MAX_INPUT_LIMIT, stdin);

            // Check for exit command or other special commands if you plan to implement them
            if (strncmp(input, "exit", 4) == 0) {
                break;
            }

            // Initialize lexer, parser, and visitor with the input
            lexer_T* lexer = init_lexer(input);
            parser_T* parser = init_parser(lexer);
            AST_T* root = parser_parse(parser, parser->scope);
            visitor_T* visitor = init_visitor();
            visitor_visit(visitor, root);
        }
    }

    return 0;
}