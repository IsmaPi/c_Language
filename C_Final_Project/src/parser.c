#include "include/parser.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>

parser_T* init_parser(lexer_T* lexer) {
    parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
    parser->lexer = lexer;
    parser->current_token = lexer_get_next_token(lexer);
    parser->prev_token = parser->current_token;

    parser->scope = init_scope();

    return parser;
}

void parser_eat(parser_T* parser, int token_type) {
    if (parser->current_token->type == token_type)
    {
        parser->prev_token = parser->current_token;
        parser->current_token = lexer_get_next_token(parser->lexer);
    }
    else
    {
        printf(
            "Unexpected token `%s`, with type %d",
            parser->current_token->value,
            parser->current_token->type
        );
        exit(1);
    }
}

AST_T* parser_parse(parser_T* parser, scope_T* scope) {
    return parser_parse_statements(parser, scope);
}

AST_T* parser_parse_statement(parser_T* parser, scope_T* scope) {
    switch (parser->current_token->type)
    {
        case TOKEN_ID: return parser_parse_id(parser, scope);
    }

    return init_ast(AST_NOOP);
}

AST_T* parser_parse_statements(parser_T* parser, scope_T* scope) {
    AST_T* compound = init_ast(AST_COMPOUND);
    compound->scope = scope;
    compound->compound_value = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_statement = parser_parse_statement(parser, scope);
    ast_statement->scope = scope;
    compound->compound_value[0] = ast_statement;
    compound->compound_size += 1;

    while (parser->current_token->type == TOKEN_SEMI)
    {
        parser_eat(parser, TOKEN_SEMI);

        AST_T* ast_statement = parser_parse_statement(parser, scope);

        if (ast_statement)
        {
            compound->compound_size += 1;
            compound->compound_value = realloc(
                compound->compound_value,
                compound->compound_size * sizeof(struct AST_STRUCT*)
            );
            compound->compound_value[compound->compound_size-1] = ast_statement;
        }
    }

    return compound;
}

AST_T* parser_parse_expr(parser_T* parser, scope_T* scope) {
    // Start with the first term
    AST_T* result = parser_parse_term(parser, scope);

    // Handle addition and subtraction
    while (parser->current_token->type == TOKEN_PLUS || parser->current_token->type == TOKEN_MINUS) {
        TokenType type = parser->current_token->type;
        parser_eat(parser, type);
        AST_T* right = parser_parse_term(parser, scope);
        result = init_ast_with_two_children(type == TOKEN_PLUS ? AST_ADD : AST_SUBTRACT, result, right);
    }

    return result;
}

AST_T* parser_parse_factor(parser_T* parser, scope_T* scope) {
    switch (parser->current_token->type) {
        case TOKEN_INT:
        case TOKEN_FLOAT:
            // Handle numeric literals
            AST_T* node = init_ast(AST_LITERAL);
            node->value = parser->current_token->value;
            parser_eat(parser, parser->current_token->type);
            return node;
        case TOKEN_LPAREN:
            // Handle expressions within parentheses
            parser_eat(parser, TOKEN_LPAREN);
            AST_T* sub_expr = parser_parse_expr(parser, scope);
            parser_eat(parser, TOKEN_RPAREN);
            return sub_expr;
        case TOKEN_ID:
            // Handle variables and function calls
            return parser_parse_id(parser, scope);
        default:
            // Error handling for unexpected tokens
            fprintf(stderr, "Unexpected token in factor: %s\n", parser->current_token->value);
            exit(EXIT_FAILURE);
    }
}

AST_T* parser_parse_term(parser_T* parser, scope_T* scope) {
    // Start with the first factor
    AST_T* result = parser_parse_factor(parser, scope);

    // Handle multiplication and division
    while (parser->current_token->type == TOKEN_STAR || parser->current_token->type == TOKEN_SLASH) {
        TokenType type = parser->current_token->type;
        parser_eat(parser, type);
        AST_T* right = parser_parse_factor(parser, scope);
        result = init_ast_with_two_children(type == TOKEN_STAR ? AST_MULTIPLY : AST_DIVIDE, result, right);
    }

    return result;
}

AST_T* parser_parse_function_call(parser_T* parser, scope_T* scope) {
    AST_T* function_call = init_ast(AST_FUNCTION_CALL);

    function_call->function_call_name = parser->prev_token->value;
    parser_eat(parser, TOKEN_LPAREN); 

    function_call->function_call_arguments = calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* ast_expr = parser_parse_expr(parser, scope);
    function_call->function_call_arguments[0] = ast_expr;
    function_call->function_call_arguments_size += 1;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);

        AST_T* ast_expr = parser_parse_expr(parser, scope);
        function_call->function_call_arguments_size += 1;
        function_call->function_call_arguments = realloc(
            function_call->function_call_arguments,
            function_call->function_call_arguments_size * sizeof(struct AST_STRUCT*)
        );
        function_call->function_call_arguments[function_call->function_call_arguments_size-1] = ast_expr;
    }
    parser_eat(parser, TOKEN_RPAREN);

    function_call->scope = scope;

    return function_call;
}

AST_T* parser_parse_variable_definition(parser_T* parser, scope_T* scope) {
    parser_eat(parser, TOKEN_ID); // var
    char* variable_definition_variable_name = parser->current_token->value;
    parser_eat(parser, TOKEN_ID); // var name
    parser_eat(parser, TOKEN_EQUALS);
    AST_T* variable_definition_value = parser_parse_expr(parser, scope);

    AST_T* variable_definition = init_ast(AST_VARIABLE_DEFINITION);
    variable_definition->variable_definition_variable_name = variable_definition_variable_name;
    variable_definition->variable_definition_value = variable_definition_value;

    variable_definition->scope = scope;

    return variable_definition;
}

AST_T* parser_parse_function_definition(parser_T* parser, scope_T* scope) {
    AST_T* ast = init_ast(AST_FUNCTION_DEFINITION);
    parser_eat(parser, TOKEN_ID); // function

    char* function_name = parser->current_token->value;
    ast->function_definition_name = calloc(
            strlen(function_name) + 1, sizeof(char)
    );
    strcpy(ast->function_definition_name, function_name);

    parser_eat(parser, TOKEN_ID); // function name

    parser_eat(parser, TOKEN_LPAREN);

    ast->function_definition_args =
        calloc(1, sizeof(struct AST_STRUCT*));

    AST_T* arg = parser_parse_variable(parser, scope);
    ast->function_definition_args_size += 1;
    ast->function_definition_args[ast->function_definition_args_size-1] = arg;

    while (parser->current_token->type == TOKEN_COMMA)
    {
        parser_eat(parser, TOKEN_COMMA);

        ast->function_definition_args_size += 1;

        ast->function_definition_args =
            realloc(
                    ast->function_definition_args,
                    ast->function_definition_args_size * sizeof(struct AST_STRUCT*)
                   );

        AST_T* arg = parser_parse_variable(parser, scope);
        ast->function_definition_args[ast->function_definition_args_size-1] = arg;
    }

    parser_eat(parser, TOKEN_RPAREN);
    
    parser_eat(parser, TOKEN_LBRACE);
    
    ast->function_definition_body = parser_parse_statements(parser, scope);

    parser_eat(parser, TOKEN_RBRACE);

    ast->scope = scope;

    return ast;
}

AST_T* parser_parse_variable(parser_T* parser, scope_T* scope) {
    char* token_value = parser->current_token->value;
    parser_eat(parser, TOKEN_ID); // var name or function call name

    if (parser->current_token->type == TOKEN_LPAREN)
        return parser_parse_function_call(parser, scope);

    AST_T* ast_variable = init_ast(AST_VARIABLE);
    ast_variable->variable_name = token_value;

    ast_variable->scope = scope;

    return ast_variable;
}

AST_T* parser_parse_string(parser_T* parser, scope_T* scope) {
    AST_T* ast_string = init_ast(AST_STRING);
    ast_string->string_value = parser->current_token->value;

    parser_eat(parser, TOKEN_STRING);

    ast_string->scope = scope;

    return ast_string;
}

AST_T* parser_parse_id(parser_T* parser, scope_T* scope) {
    if (strcmp(parser->current_token->value, "var") == 0)
    {
        return parser_parse_variable_definition(parser, scope);
    }
    else
    if (strcmp(parser->current_token->value, "function") == 0)
    {
        return parser_parse_function_definition(parser, scope);
    }
    else
    {
        return parser_parse_variable(parser, scope);
    }
}

AST_T* parse_integration_or_derivation(parser_T* parser) {
    if (parser->current_token->type == TOKEN_INTEGRATE || parser->current_token->type == TOKEN_DERIVATE) {
        TokenType operation = parser->current_token->type;
        parser_eat(parser, operation); // Consume 'integrate' or 'derivate'
        parser_eat(parser, TOKEN_LPAREN);
        
        AST_T* expr = parse_expression(parser); // Parse the expression inside

        parser_eat(parser, TOKEN_RPAREN); 

        AST_T* ast = init_ast(operation == TOKEN_INTEGRATE ? AST_INTEGRATE : AST_DERIVATIVE);
        if (operation == TOKEN_INTEGRATE) {
            ast->integrate_expr = expr;
        } else {
            ast->derivate_expr = expr;
        }
        return ast;
    }
}
<
