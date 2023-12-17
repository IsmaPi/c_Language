#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"
#include "AST.h"
#include "scope.h"

// Parser structure definition
typedef struct PARSER_STRUCT {
    lexer_T* lexer;           // The lexer for tokenizing input
    token_T* current_token;   
    token_T* prev_token;      
    scope_T* scope;
} parser_T;

// Function prototypes for the parser
parser_T* init_parser(lexer_T* lexer);
void parser_eat(parser_T* parser, int token_type);

// Core parsing functions
AST_T* parser_parse(parser_T* parser, scope_T* scope);
AST_T* parser_parse_statement(parser_T* parser, scope_T* scope);
AST_T* parser_parse_statements(parser_T* parser, scope_T* scope);

// Parsing functions for expressions, terms, factors, etc.
AST_T* parser_parse_expr(parser_T* parser, scope_T* scope);
AST_T* parser_parse_factor(parser_T* parser, scope_T* scope);
AST_T* parser_parse_term(parser_T* parser, scope_T* scope);

// Parsing functions for function calls, variable and function definitions
AST_T* parser_parse_function_call(parser_T* parser, scope_T* scope);
AST_T* parser_parse_variable_definition(parser_T* parser, scope_T* scope);
AST_T* parser_parse_function_definition(parser_T* parser, scope_T* scope);
AST_T* parser_parse_variable(parser_T* parser, scope_T* scope);

// Parsing functions for string literals and identifiers
AST_T* parser_parse_string(parser_T* parser, scope_T* scope);
AST_T* parser_parse_id(parser_T* parser, scope_T* scope);

#endif
// Parsing functions for string literals and identifiers
AST_T* parser_parse_string(parser_T* parser, scope_T* scope);
AST_T* parser_parse_id(parser_T* parser, scope_T* scope);

#endif
